#ifndef TIMETOSAMPLEBOX_H
#define TIMETOSAMPLEBOX_H

#include "BaseBox.h"

class TimeToSampleBox : public BaseBox
{
public:
    TimeToSampleBox(uint32_t type, uint32_t size);

public:
    virtual int Parse(class mp4Parser* parser, uint32_t start_pos);
    string GetDescription();

private:
    Stream* s;
};

#endif // TIMETOSAMPLEBOX_H
