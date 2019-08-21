#include "ChunkOffsetBox.h"

ChunkOffsetBox::ChunkOffsetBox(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{

}

int ChunkOffsetBox::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    io->Read8(); //version
    io->Read24(); //flags

    s = parser->streams[parser->stream_num-1];
    s->stco_count = io->Read32();
    s->stco_data = new uint64_t[s->stco_count];
    for(int i=0;i<s->stco_count;i++)
    {
        if(this->type == FOURCC_stco)
            s->stco_data[i] = io->Read32();
        else if(this->type == FOURCC_co64)
            s->stco_data[i] = io->Read64();
    }
    return 0;
}

string ChunkOffsetBox::GetDescription()
{
    string desc = BasePosition::GetDescription();
    if(this->s != NULL)
    {
        char tmp[128];
        sprintf(tmp, "entry_count = %d\n", s->stco_count);
        for(int i=0;i<s->stco_count;i++)
        {
            sprintf(tmp, "index = %d, chunk_offset = %lld\n", i, s->stco_data[i]);
            desc += tmp;
        }
    }
    return desc;
}
