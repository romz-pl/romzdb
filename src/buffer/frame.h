#ifndef ROMZDB_FRAME_H
#define ROMZDB_FRAME_H

//
// It contains the data about a page in the buffer manager.
//

#include <cstdint>
#include "pageid.h"
#include "disk.h"

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

    void Read( const Disk& disk, PageId pageId );
    void Write( const Disk &disk );




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
