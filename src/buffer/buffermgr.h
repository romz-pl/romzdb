#ifndef ROMZDB_BUFFCLOCK_H
#define ROMZDB_BUFFCLOCK_H

#include <vector>
#include <map>
#include "disk/diskblock.h"
#include "disk/pageid.h"
#include "frame.h"
#include "disk/space.h"

class BufferMgr
{
public:
    BufferMgr( Space& space, std::uint32_t frame_no );
    ~BufferMgr();

    DiskBlock* pin( PageId page_id );
    void unpin( PageId page_id, bool dirty );
    std::pair< PageId, DiskBlock * > alloc( );
    void dispose( PageId page_id );
    void flush();

private:
    void advance();
    void allocBuff();

private:
    // clock hand for clock algorithm
    Frame* m_clock_hand;

    // mapping form PageId to FrameId
    std::map< PageId, Frame* > m_map;

    // buffer pool
    std::vector< Frame > m_frame;

    // Statistics about buffer pool usage
    // BufStats bufStats;

    Space& m_space;


};


#endif
