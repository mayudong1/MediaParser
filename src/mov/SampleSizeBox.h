#ifndef SAMPLESIZEBOX_H
#define SAMPLESIZEBOX_H

#include "BaseBox.h"

class SampleSizeBox : public BaseBox
{
public:
    SampleSizeBox(uint32_t type, uint32_t size);

    virtual int Parse(class mp4Parser* parser, uint32_t start_pos);
    string GetDescription();
private:
    Stream* s;
};

#endif // SAMPLESIZEBOX_H
