#include "SampleToChunkBox.h"

SampleToChunkBox::SampleToChunkBox(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{

}

int SampleToChunkBox::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    io->Read8(); //version
    io->Read24(); //flags

    s = parser->streams[parser->stream_num-1];
    s->stsc_count = io->Read32();
    s->stsc_data = new stsc_entry[s->stsc_count];
    for(int i=0;i<s->stsc_count;i++)
    {
        s->stsc_data[i].first_chunk = io->Read32();
        s->stsc_data[i].samples_per_chunk = io->Read32();
        s->stsc_data[i].sample_description_index = io->Read32();
    }
    return 0;
}

string SampleToChunkBox::GetDescription()
{
    string desc = BasePosition::GetDescription();
    if(this->s != NULL)
    {
        char tmp[128];
        sprintf(tmp, "entry_count = %d\n", s->stsc_count);
        desc += tmp;
        for(int i=0;i<s->stsc_count;i++)
        {
            sprintf(tmp, "first_chunk = %d, samples_per_chunk = %d, sample_description_index = %d\n",
                    s->stsc_data[i].first_chunk,
                    s->stsc_data[i].samples_per_chunk,
                    s->stsc_data[i].sample_description_index);
            desc += tmp;
        }
    }
    return desc;
}
