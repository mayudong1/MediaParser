#include "BasePosition.h"

BasePosition::BasePosition()
{

}

void BasePosition::SetPosition(uint32_t start, uint32_t length)
{
    this->start_pos = start;
    this->length = length;
}

string BasePosition::GetDescription()
{
    char tmp[128];
    sprintf(tmp, "offset = %d, len = %d\n", start_pos, length);
    return tmp;
}
