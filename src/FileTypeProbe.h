#ifndef FILETYPEPROBE_H
#define FILETYPEPROBE_H

enum MediaFileType
{
    MOV,
    FLV,
    UNKNOWN
};

class FileTypeProbe
{
public:
    FileTypeProbe();

    MediaFileType Probe(const char* filename);
};

#endif // FILETYPEPROBE_H
