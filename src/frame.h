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

    bool IsEqual( PageId pageId ) const;
    bool IsPinned() const;

    DiskBlock* GetBlock();


    void Read( const DiskSpaceMgr& ds, PageId pageId );
    void Write( const DiskSpaceMgr &ds );

    void UnpinPage();

/*    void MarkDirty( );







    void Print() const;
*/
private:
    // Invalid page ID. This page ID must not be used!
    // static const PageId m_invalidPageId;

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
