#ifndef ROMZDB_SLOT_H
#define ROMZDB_SLOT_H

#include "buffer/page.h"
#include "pageoffset.h"

class Slot
{
public:
    Slot( PageOffset offset, PageOffset length );
    ~Slot() = default;

    void ToPage( char *& dest ) const;
    static Slot FromPage(const char *& src );

    void SetInvalid();
    bool IsValid() const;

// private:
    // Record offset
    PageOffset m_offset;

    // Record length
    PageOffset m_length;
};

#endif
