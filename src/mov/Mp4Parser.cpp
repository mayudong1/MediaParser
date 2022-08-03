#include <stdlib.h>
#include "Mp4Parser.h"
#include "BaseBox.h"
#include "DefaultContainerBox.h"
#include "MovieHeaderBox.h"
#include "TrackBox.h"
#include "TrackHeaderBox.h"
#include "MediaHeaderBox.h"
#include "HandlerRefBox.h"
#include "SampleDescBox.h"
#include "TimeToSampleBox.h"
#include "SyncSampleBox.h"
#include "CompositionOffsetBox.h"
#include "ChunkOffsetBox.h"
#include "SampleToChunkBox.h"
#include "SampleSizeBox.h"
#include "MetaBox.h"
#include "PrimaryItemBox.h"
#include "ItemInfoBox.h"
#include "ItemReferenceBox.h"
#include "ItemLocationBox.h"
#include "ItemPropertyContainerBox.h"
#include "ItemPropertyAssociationBox.h"
#include "EditListBox.h"

#include <stdio.h>
#include <QtGlobal>

mp4Parser::mp4Parser()
{
    this->io = NULL;
    this->init();
}

mp4Parser::mp4Parser(FileReader* io)
{
    this->io = io;
    this->init();
}

void mp4Parser::init()
{
    this->inner_io = NULL;
    dummy = new BaseBox(0, 0);
    memset(debugInfo, 0, 4096);

    stream_num = 0;
    memset(streams, 0, sizeof(Stream*)*MAX_STREAM_COUNT);

    duration = 0;
    timescale = 1;
    width = 0;
    height = 0;
}

mp4Parser::~mp4Parser()
{
    if(inner_io != NULL)
    {
        delete inner_io;
        inner_io = NULL;
    }
    if(dummy != NULL)
    {
        DestroyBox(dummy);
    }
    for(int i=0;i<this->stream_num;i++)
    {
        DeleteStream(this->streams[i]);
        this->streams[i] = NULL;
    }
}

BaseBox* mp4Parser::AllocBox(uint32_t type, uint32_t size)
{
    BaseBox* box = NULL;
    switch(type)
    {
    case FOURCC_moov:
    case FOURCC_edts:
    case FOURCC_mdia:
    case FOURCC_minf:
	case FOURCC_dinf:
    case FOURCC_stbl:
    case FOURCC_udta:
    case FOURCC_iprp:
    case FOURCC_sinf:
    case FOURCC_schi:
        box = new DefaultContainerBox(type, size);
        break;
    case FOURCC_elst:
        box = new EditListBox(type, size);
        break;
    case FOURCC_mvhd:
        box = new MovieHeaderBox(type, size);
        break;
    case FOURCC_trak:
        box = new TrackBox(type, size);
        break;
    case FOURCC_tkhd:
        box = new TrackHeaderBox(type, size);
        break;
    case FOURCC_mdhd:
        box = new MediaHeaderBox(type, size);
        break;
    case FOURCC_hdlr:
        box = new HandlerRefBox(type, size);
        break;
    case FOURCC_stsd:
        box = new SampleDescBox(type, size);
        break;
    case FOURCC_stts:
        box = new TimeToSampleBox(type, size);
        break;
    case FOURCC_stss:
        box = new SyncSampleBox(type, size);
        break;
    case FOURCC_ctts:
        box = new CompositionOffsetBox(type, size);
        break;
    case FOURCC_stco:
    case FOURCC_co64:
        box = new ChunkOffsetBox(type, size);
        break;
    case FOURCC_stsc:
        box = new SampleToChunkBox(type, size);
        break;
    case FOURCC_stsz:
    case FOURCC_stz2:
        box = new SampleSizeBox(type, size);
        break;
    case FOURCC_meta:
        box = new MetaBox(type, size);
        break;
    case FOURCC_pitm:
        box = new PrimaryItemBox(type, size);
        break;
    case FOURCC_iinf:
        box = new ItemInfoBox(type, size);
        break;
    case FOURCC_iref:
        box = new ItemReferenceBox(type, size);
        break;
    case FOURCC_iloc:
        box = new ItemLocationBox(type, size);
        break;
    case FOURCC_ipco:
        box = new ItemPropertyContainerBox(type, size);
        break;
    case FOURCC_ipma:
        box = new ItemPropertyAssociationBox(type, size);
        break;
    default:
        box = new BaseBox(type, size);
        break;
    }

    return box;
}

BaseBox* mp4Parser::ReadBox(uint32_t start_pos)
{
	int index = 0;
    uint32_t size = io->Read32();
    uint32_t type = io->Read32();
	index = 8;
	if (size == 1)
	{
		index += 8;
		size = io->Read64();
	}
    BaseBox* box = AllocBox(type, size);

    box->SetPosition(start_pos, size);
    box->Parse(this, start_pos+index);

    return box;
}

void mp4Parser::AddBox(BaseBox* root, BaseBox* new_box)
{
    if(root == NULL || new_box == NULL)
            return;

    root->AddChild(new_box);
}

void mp4Parser::DestroyBox(BaseBox* root)
{
    if(root->childs)
    {
        DestroyBox(root->childs);
    }
    if(root->next)
    {
        DestroyBox(root->next);
    }

    delete root;
}

BaseBox* mp4Parser::GetParseResult()
{
    return dummy;
}

Stream* mp4Parser::AddStream()
{
    Stream* s = new Stream;
    memset(s, 0, sizeof(Stream));
    s->index = this->stream_num;
    this->streams[this->stream_num] = s;
    this->stream_num++;
    return s;
}

void mp4Parser::DeleteStream(Stream* s)
{
    if(!s)
        return;

    if(s->handler)
        delete s->handler;
    if(s->stts_data)
        delete[] s->stts_data;
    if(s->stss_data)
        delete[] s->stss_data;
    if(s->ctts_data)
        delete[] s->ctts_data;
    if(s->stco_data)
        delete[] s->stco_data;
    if(s->stsc_data)
        delete[] s->stsc_data;
    if(s->stsz_data)
        delete[] s->stsz_data;
    if(s->sample_position)
        delete[] s->sample_position;
    if(s->elst_data)
        delete[] s->elst_data;
    if(s->sample_dts)
        delete[] s->sample_dts;
    if(s->sample_pts)
        delete[] s->sample_pts;

    delete s;
}

int mp4Parser::Parse(const char* filename)
{
    if(io == NULL)
    {
        inner_io = new FileReader();
        io = inner_io;
    }

    int ret = io->Open(filename);
    if(ret != 0)
    {
        return -1;
    }

    uint32_t cur_pos = 0;
    int file_size = io->GetLength();
    if(file_size <= 8)
    {
        return -1;
    }
    io->Read32();
    uint32_t ftyp = io->Read32();
    if(ftyp != FOURCC_ftyp)
    {
        return -1;
    }
    io->SetPos(0);

    while(cur_pos < file_size)
    {
        BaseBox* box = ReadBox(cur_pos);
        this->dummy->AddChild(box);
        cur_pos += box->size;
        io->SetPos(cur_pos);
    }

    for(int i=0;i<this->stream_num;i++)
    {
        this->GetSamplePosition(this->streams[i]);
        this->GetSampleTimeStamp(this->streams[i]);
    }
    return 0;
}

//pts[index] >= elst.media_time
int mp4Parser::FindFirstDisplaySample(Stream* s)
{
    int sample_count = s->stsz_count;
    uint64_t start_media_time = 0;
    if(s->elst_data != NULL)
        start_media_time = s->elst_data[0].media_time;

    for(int i=0;i<sample_count;i++)
    {
        if(s->sample_pts[i] >= start_media_time)
            return i;
    }
    return 0;
}

void mp4Parser::GetSampleTimeStamp(Stream* s)
{
    int sample_count = s->stsz_count;
    if(sample_count <= 0)
        return;

    s->sample_dts = new int64_t[sample_count];
    s->sample_pts = new int64_t[sample_count];
    int index = 1;
    s->sample_dts[0] = 0;
    for(int i=0;i<s->stts_count;i++)
    {
        for(int j=0;j<s->stts_data[i].sample_count;j++)
        {
            int delta = s->stts_data[i].sample_delta;
            s->sample_dts[index] = s->sample_dts[index-1] + delta;
            index++;
            if(index == sample_count)
                break;
        }
    }
    index = 0;
    for(int i=0;i<s->ctts_count;i++)
    {
        for(int j=0;j<s->ctts_data[i].sample_count;j++)
        {
            s->sample_pts[index] = s->sample_dts[index] + s->ctts_data[i].sample_offset;
            index++;
            if(index == sample_count)
                break;
        }
    }
    if(s->ctts_count == 0)
    {
        for(int i=0;i<sample_count;i++)
        {
            s->sample_pts[i] = s->sample_dts[i];
        }
    }
    int first_sample_index = FindFirstDisplaySample(s);
    uint64_t first_sample_pts = s->sample_pts[first_sample_index];
    for(int i=0;i<sample_count;i++)
    {
        s->sample_dts[i] -= first_sample_pts;
        s->sample_pts[i] -= first_sample_pts;
    }
}

void mp4Parser::GetSamplePosition(Stream* s)
{
    int sample_count = s->stsz_count;
	int chunk_count = s->stco_count;

    if(sample_count > 0)
    {
        s->sample_position = new uint64_t[sample_count];
    }

	int remain_chunk_count = chunk_count;
	int sample_index = 0;
    for(int i=0;i<s->stsc_count;i++)
    {
		int c_count = 0;
		if (i != s->stsc_count - 1)
		{
			c_count = s->stsc_data[i + 1].first_chunk - s->stsc_data[i].first_chunk;
			remain_chunk_count -= c_count;
		}
		else
		{
			c_count = remain_chunk_count;
		}
		for (int j = 0; j < c_count; j++)
		{
			int chunk_index = s->stsc_data[i].first_chunk + j;
			uint64_t offset = s->stco_data[chunk_index - 1];
			for (int k = 0; k < s->stsc_data[i].samples_per_chunk; k++)
			{
				s->sample_position[sample_index] = offset;
				offset += s->stsz_data[sample_index];
				sample_index++;
				if (sample_index > sample_count)
					return;
			}
		}
    }
}

Stream* mp4Parser::GetVideoStream()
{
    for(int i=0;i<this->stream_num;i++)
    {
        if(this->streams[i]->type == HANDLER_VIDEO)
            return this->streams[i];
    }
    return NULL;
}

Stream* mp4Parser::GetAudioStream()
{
    for(int i=0;i<this->stream_num;i++)
    {
        if(this->streams[i]->type == HANDLER_AUDIO)
            return this->streams[i];
    }
    return NULL;
}
