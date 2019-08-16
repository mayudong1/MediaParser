#include "SyncSampleBox.h"

SyncSampleBox::SyncSampleBox(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{
    this->s = NULL;
}

int SyncSampleBox::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    io->Read8(); //version
    io->Read24(); //flags

    s = parser->streams[parser->stream_num-1];
    s->stss_count = io->Read32();
    s->stss_data = new uint32_t[s->stss_count];
    for(int i=0;i<s->stss_count;i++)
    {
        s->stss_data[i] = io->Read32();
    }
    return 0;
}

string SyncSampleBox::GetDescription()
{
    string desc = BasePosition::GetDescription();
    if(this->s != NULL)
    {
        char tmp[128];
        sprintf(tmp, "stss_count = %d\n", s->stss_count);
        desc += tmp;
        for(int i=0;i<s->stss_count;i++)
        {
            sprintf(tmp, "%d\n", s->stss_data[i]);
            desc += tmp;
        }
    }
    return desc;
}
