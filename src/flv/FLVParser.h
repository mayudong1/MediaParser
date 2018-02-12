#ifndef FLVPARSER_H
#define FLVPARSER_H

#include <stdint.h>
#include "FLVDefine.h"
#include "../FileReader.h"

class FLVParser
{
public:
    FLVParser();
    FLVParser(FileReader* io);
    ~FLVParser();
public:
    int Parse(const char* filename);


private:
    void init();
    void destory();

    int parseFLVHeader();
    int parseFLVTags();
private:
    FileReader* io;
    FileReader* inner_io;

public:
    FLVStruct* flv;
};

#endif // FLVPARSER_H
