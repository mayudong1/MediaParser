#include "MovieHeaderBox.h"

MovieHeaderBox::MovieHeaderBox(uint32_t type, uint32_t size)
    :BaseBox(type, size)
{

}

MovieHeaderBox::~MovieHeaderBox()
{

}

int MovieHeaderBox::Parse(class mp4Parser* parser, uint32_t start_pos)
{
    FileReader* io = parser->io;
    io->SetPos(start_pos);

    uint64_t creation_time, modification_time;
    uint32_t timescale;
    uint64_t duration;

    uint8_t version = io->Read8();
    io->Read24(); //flag

    if(version == 1)
    {
        creation_time = io->Read64();
        modification_time = io->Read64();
        timescale = io->Read32();
        duration = io->Read64();
    }
    else
    {
        creation_time = io->Read32();
        modification_time = io->Read32();
        timescale = io->Read32();
        duration = io->Read32();
    }
    this->duration = duration;
    this->timescale = timescale;
    parser->duration = this->duration;
    parser->timescale = this->timescale;
    return 0;
}

string MovieHeaderBox::GetDescription()
{
    string desc = BasePosition::GetDescription();
    char tmp[128];
    sprintf(tmp, "timescale = %d (0x%X)\nduration = %lld (0x%X)\n", this->timescale, this->timescale,
            this->duration, this->duration);
    desc += tmp;
    return desc;
}
