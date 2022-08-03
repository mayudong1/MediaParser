#include "AudioSampleEntry.h"

AudioSampleEntry::AudioSampleEntry(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{

}

AudioSampleEntry::~AudioSampleEntry()
{

}

int AudioSampleEntry::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    io->Skip(6);
    io->Read16(); //data_reference_index

    io->Skip(8); //reserved

    Stream* s = parser->streams[parser->stream_num-1];
    s->channel_count = io->Read16();
    s->sample_size = io->Read16();

    io->Read16();
    io->Read16();

    s->sample_rate = io->Read32()>>16;

    //below other box
    int cur_pos = io->GetPos();
    while(cur_pos-start_pos < this->size-16){
        BaseBox* box = parser->ReadBox(cur_pos);
        parser->AddBox(this, box);
        cur_pos += box->size;
        io->SetPos(cur_pos);
    }

    return 0;
}
