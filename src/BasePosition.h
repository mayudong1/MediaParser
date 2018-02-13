#ifndef BASEPOSITION_H
#define BASEPOSITION_H

#include <stdint.h>
#include <string>

using namespace std;

class BasePosition
{
public:
    BasePosition();

public:
    void SetPosition(uint32_t start, uint32_t length);
    virtual string GetDescription();

public:
    uint32_t start_pos;
    uint32_t length;
};

#endif // BASEPOSITION_H
