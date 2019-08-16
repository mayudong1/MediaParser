#include "CompositionOffsetBox.h"

CompositionOffsetBox::CompositionOffsetBox(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{
    this->s = NULL;
}

int CompositionOffsetBox::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    io->Read8(); //version
    io->Read24(); //flags

    s = parser->streams[parser->stream_num-1];
    s->ctts_count = io->Read32();
    s->ctts_data = new ctts_entry[s->ctts_count];
    for(int i=0;i<s->ctts_count;i++)
    {
        s->ctts_data[i].sample_count = io->Read32();
        s->ctts_data[i].sample_offset = io->Read32();
    }
    return 0;
}

string CompositionOffsetBox::GetDescription()
{
    string desc = BasePosition::GetDescription();
    if(this->s != NULL)
    {
        char tmp[128];
        sprintf(tmp, "ctts_count = %d\n", s->ctts_count);
        desc += tmp;
        for(int i=0;i<s->ctts_count;i++)
        {
            sprintf(tmp, "count:%d, offset:%d\n", s->ctts_data[i].sample_count, s->ctts_data[i].sample_offset);
            desc += tmp;
        }
    }
    return desc;
}
