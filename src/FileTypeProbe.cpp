#include "FileTypeProbe.h"
#include "FileReader.h"

FileTypeProbe::FileTypeProbe()
{

}

MediaFileType FileTypeProbe::Probe(const char* filename)
{
    MediaFileType type = MediaFileType::UNKNOWN;
    FileReader* reader = new FileReader();
    do
    {
        int ret = reader->Open(filename);
        if(ret < 0)
        {
            break;
        }
        if(reader->GetLength() < 8)
        {
            break;
        }
        char buffer[8] = {0};
        ret = reader->ReadBuffer(buffer, 8);
        if(ret < 8)
            break;

        if(buffer[0] == 'F' && buffer[1] == 'L' && buffer[2] == 'V')
        {
            type = MediaFileType::FLV;
            break;
        }
        if(buffer[4] == 'f' && buffer[5] == 't' && buffer[6] == 'y' && buffer[7] == 'p')
        {
            type = MediaFileType::MOV;
            break;
        }
    }while(0);

    delete reader;
    return type;
}
