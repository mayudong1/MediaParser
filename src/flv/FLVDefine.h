#ifndef FLVDEFINE_H
#define FLVDEFINE_H

typedef struct FLVHeader
{
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
    FLVTagHeader header;

    uint32_t preTagSize;
    FLVTag* next;
}FLVTag;

typedef struct FLVStruct
{
    FLVHeader header;
    uint32_t firstTagSize;
    FLVTag* tagList;
}FLVStruct;



#endif // FLVDEFINE_H
