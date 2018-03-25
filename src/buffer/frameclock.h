#ifndef ROMZDB_FRAMECLOCK_H
#define ROMZDB_FRAMECLOCK_H

//
// It contains the data about a page in the buffer manager.
//

#include <cstdint>
#include "disk/pageid.h"
#include "frameid.h"
#include "disk/diskblock.h"

class FrameClock
{
    friend class BuffClock;
public:
    FrameClock( );
    ~FrameClock() = default;

    void clear();
    void set( PageId page_id );

private:
    DiskBlock m_block;

    // Page to which corresponding frame is assigned
    PageId m_page_id;

    // Number of times this page has been pinned
    std::uint16_t m_pin_count;

    // True if page is dirty;  false otherwise
    bool m_dirty;

    // True if page is valid
    bool m_valid;

    // Has this buffer frame been reference recently
    bool m_refbit;

};

#endif
