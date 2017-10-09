#ifndef ROMZDB_FRAME_H
#define ROMZDB_FRAME_H

//
// It contains the data about a page in the buffer manager.
//

#include <cstdint>
#include "page.h"
#include "diskspacemgr.h"

class Frame
{
public:
    Frame( );

    bool IsEqual( PageId pageId ) const;

    void MarkDirty( );

    bool IsPinned() const;
    void UnpinPage();

    void Read( const DiskSpaceMgr& ds, PageId pageId );
    void Write( const DiskSpaceMgr &ds );

    Page* GetPage();

private:
    // Invalid page ID. This page ID must not be used!
    static const PageId m_invalidPageId;

    // Page identyfier for this page
    PageId m_pageId;

    // The page stored in the frame
    Page m_page;

    // pin count
    std::uint8_t m_pinCount;

    // true if page is dirty
    bool m_dirty;

};

#endif
