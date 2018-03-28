#ifndef ROMZDB_BUFFCLOCK_H
#define ROMZDB_BUFFCLOCK_H

#include <vector>
#include <stack>
#include "disk/diskblock.h"
#include "disk/pageid.h"
#include "frame.h"
#include "disk/space.h"
#include "util/bimap.h"

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
    void advance_clock_hand();
    void find_frame_for_replacement();
    DiskBlock* replace_frame( PageId page_id );

private:
    // clock hand for clock algorithm
    Frame* m_clock_hand;

    // mapping form PageId to FrameId
    bimap< PageId, Frame* > m_bimap;

    // buffer pool
    std::vector< Frame > m_pool;

    std::stack< Frame* > m_free;

    // Statistics about buffer pool usage
    // BufStats bufStats;

    Space& m_space;
};


#endif
