#ifndef FLVDEFINE_H
#define FLVDEFINE_H

#include "../BasePosition.h"

typedef struct FLVHeader
{
    BasePosition FLVHeader_pos;
    uint8_t signature[4];
    uint8_t version;
    bool hasVideo;
    bool hasAudio;
    uint8_t headerLen;
}FLVHeader;

typedef struct FLVTagHeader
{
    bool encrypted;
    uint8_t type;
    uint32_t dataSize;
    uint32_t timestamp;
    uint32_t streamId;
}FLVTagHeader;

typedef struct FLVTag
{
    BasePosition FLVTag_pos;
    FLVTagHeader header;

    uint32_t preTagSize;
    BasePosition preTagSize_pos;
    FLVTag* next;
}FLVTag;

typedef struct FLVStruct
{
    FLVHeader header;
    uint32_t firstTagSize;
    FLVTag* tagList;
}FLVStruct;



#endif // FLVDEFINE_H
