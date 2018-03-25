#ifndef ROMZDB_BUFFCLOCK_H
#define ROMZDB_BUFFCLOCK_H

#include <vector>
#include <map>
#include "disk/diskblock.h"
#include "disk/pageid.h"
#include "frameclock.h"
#include "disk/space.h"

class BuffClock
{
public:
    BuffClock( Space& space, std::uint32_t frame_no );
    ~BuffClock();

    DiskBlock* read( const PageId page_id );
    void unpin( const PageId page_id, const bool dirty );
    DiskBlock* alloc( PageId& page_id );
    void dispose( const PageId page_id );
    void flush();

private:
    void advance();
    void allocBuff();

private:
    // clock hand for clock algorithm
    FrameClock* m_clock_hand;

    // mapping form PageId to FrameId
    std::map< PageId, FrameClock* > m_map;

    // buffer pool
    std::vector< FrameClock > m_frame;

    // Statistics about buffer pool usage
    // BufStats bufStats;

    Space& m_space;


};


#endif
