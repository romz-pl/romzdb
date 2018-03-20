#ifndef ROMZDB_FRAME_H
#define ROMZDB_FRAME_H

//
// It contains the data about a page in the buffer manager.
//

#include <cstdint>
#include "pageid.h"
#include "diskblock.h"
#include "diskspacemgr.h"

class Frame
{
public:
    Frame( );
    ~Frame() = default;

    PageId GetPageId( ) const;

    bool IsPinned() const;
    void UnpinPage();

    void MarkDirty( );

    DiskBlock* GetBlock();

    void Read( const DiskSpaceMgr& ds, PageId pageId );
    void Write( const DiskSpaceMgr &ds );




private:

    // Page identyfier associated with the disk block
    PageId m_pageId;

    // The diskblock stored in the frame
    DiskBlock m_block;

    // pin count
    std::uint8_t m_pinCount;

    // true if page is dirty
    bool m_dirty;

};

#endif
