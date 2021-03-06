#ifndef ROMZDB_FRAMECLOCK_H
#define ROMZDB_FRAMECLOCK_H

//
// It contains the data about a page in the buffer manager.
//

#include <cstdint>
#include <map>
#include "disk/pageid.h"
#include "disk/diskblock.h"
#include "disk/space.h"

class Frame
{
    // friend class BufferMgr;
public:
    Frame( );
    ~Frame() = default;

    void flush( Space& space , PageId page_id);
    void dispose( Space& space , PageId page_id );
    void unpin( bool dirty );
    DiskBlock* pin();

    void write( Space& space , PageId page_id );
    DiskBlock* read( Space& space, PageId page_id );

    bool is_for_replacement( std::uint32_t &countPinned );

private:
    void set();

private:
    DiskBlock m_block;

    // Number of times this page has been pinned
    std::uint16_t m_pin_count;

    // True if page is dirty;  false otherwise
    bool m_dirty;

    // Has this buffer frame been reference recently
    bool m_refbit;

};

#endif
