#ifndef ROMZDB_SLOT_H
#define ROMZDB_SLOT_H

#include "page.h"

class Slot
{
public:
    Slot( PageOffset offset, PageOffset length );
    ~Slot() = default;

// private:
    // Record offset
    PageOffset m_offset;

    // Record length
    PageOffset m_length;
};

#endif
