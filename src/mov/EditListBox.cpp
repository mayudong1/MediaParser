#include "EditListBox.h"

EditListBox::EditListBox(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{

}

int EditListBox::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    int version = io->Read8(); //version
    io->Read24(); //flags

    s = parser->streams[parser->stream_num-1];
    s->elst_count = io->Read32();
    s->elst_data = new elst_entry[s->elst_count];

    for(int i=0;i<s->elst_count;i++)
    {
        if(version == 1)
        {
            uint64_t segment_duration = io->Read64();
            int64_t media_time = io->Read64();
            s->elst_data[i].segment_duration = segment_duration;
            s->elst_data[i].media_time = media_time;
        }
        else
        {
            uint32_t segment_duration = io->Read32();
            int32_t media_time = io->Read32();
            s->elst_data[i].segment_duration = segment_duration;
            s->elst_data[i].media_time = media_time;
        }
        s->elst_data[i].media_rate_integer = io->Read16();
        s->elst_data[i].media_rate_fraction = io->Read16();
    }
    return 0;
}

string EditListBox::GetDescription()
{
    string desc = BasePosition::GetDescription();
    char tmp[128];
    sprintf(tmp, "elst_count = %d\n", s->elst_count);
    desc += tmp;
    for(int i=0;i<s->elst_count;i++)
    {
        sprintf(tmp, "segment_duration = %lld\n"
                     "media_time = %lld\n"
                     "media_rate_integer = %d\n\n",
                s->elst_data[i].segment_duration,
                s->elst_data[i].media_time,
                s->elst_data[i].media_rate_integer);
        desc += tmp;
    }
    return desc;
}
