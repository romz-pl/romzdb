#ifndef ROMZDB_BUFFERMGR_H
#define ROMZDB_BUFFERMGR_H


#include <vector>
#include "frame.h"
#include "disk/space.h"


class BufferMgr
{

public:
    BufferMgr( Space& space, std::size_t frameNo );
    ~BufferMgr();

    DiskBlock* Get( PageId pageId, bool multiplePins );
    void Unpin( PageId pageId );

    PageId GetNew();

    void MarkDirty( PageId pageId );

private:
    DiskBlock* GetFromDisk( PageId pageId );
    Frame& FindFrame( PageId pageId );

    void Flush( );

private:
    Space& m_space;

    // Pool of frames stored in the buffer manager
    std::vector< Frame > m_pool;

};

#endif
