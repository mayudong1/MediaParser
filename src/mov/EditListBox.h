#ifndef EDITLISTBOX_H
#define EDITLISTBOX_H

#include "BaseBox.h"
class EditListBox : public BaseBox
{
public:
    EditListBox(uint32_t type, uint32_t size);

    virtual int Parse(class mp4Parser* parser, uint32_t start_pos);
    string GetDescription();
private:
    Stream* s;
};

#endif // EDITLISTBOX_H
