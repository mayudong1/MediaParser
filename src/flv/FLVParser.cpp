#include "FLVParser.h"
#include <string.h>

FLVParser::FLVParser()
{
    this->io = NULL;
    this->init();
}

FLVParser::FLVParser(FileReader* io)
{
    this->io = io;
    this->init();
}

FLVParser::~FLVParser()
{
    if(inner_io != NULL)
    {
        delete inner_io;
        inner_io = NULL;
    }
}

void FLVParser::init()
{
    flv = NULL;
}

void FLVParser::destory()
{

}

int FLVParser::Parse(const char* filename)
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

    flv = new FLVStruct();
    if(flv == NULL)
    {
        return -1;
    }

    int curPos = 0;
    curPos = parseFLVHeader();
    if(curPos < 0)
    {
        return -1;
    }
    flv->firstTagSize = io->Read32();

    curPos = parseFLVTags();
    if(curPos < 0)
        return -1;

    return 0;
}

int FLVParser::parseFLVHeader()
{
    flv->header.signature[0] = io->Read8();
    flv->header.signature[1] = io->Read8();
    flv->header.signature[2] = io->Read8();
    if(strcmp((char*)flv->header.signature, "FLV") != 0)
    {
        return -1;
    }
    flv->header.version = io->Read8();

    uint8_t flag = io->Read8();
    flv->header.hasAudio = (flag>>2) & 0x01;
    flv->header.hasVideo = flag & 0x01;
    flv->header.headerLen = io->Read32();
    if(flv->header.headerLen != 9)
    {
        return -1;
    }

    return 0;
}

int FLVParser::parseFLVTags()
{
    flv->tagList = new FLVTag();
    FLVTag* p = flv->tagList;
    while(1)
    {
        FLVTag* tag = new FLVTag();
        uint8_t type = io->Read8();
        tag->header.encrypted = (type>>5) & 0x01;
        tag->header.type = type & 0x1f;
        tag->header.dataSize = io->Read24();

        uint32_t timestamp = io->Read24();
        uint8_t timestampEx = io->Read8();
        tag->header.timestamp = timestampEx<<24 | timestamp;
        tag->header.streamId = io->Read24();

        io->Skip(tag->header.dataSize);
        tag->preTagSize = io->Read32();
        p->next = tag;
        p = p->next;
        if(io->isEOF())
        {
            break;
        }
    }
    return 0;
}
