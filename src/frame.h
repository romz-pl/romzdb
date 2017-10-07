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

    void SetDirty( bool dirty );

    bool IsPinned() const;
    void IncPin();
    void DecPin();

    void Read( const DiskSpaceMgr& ds, PageId pageId );
    void Write( const DiskSpaceMgr &ds );

    Page* GetPage();

// private:
    // Page identyfier for this page
    PageId m_pageId;

private:
    // The page stored in the frame
    Page m_page;

    // pin count
    std::uint8_t m_pinCount;

    // true if page is dirty
    bool m_dirty;

};

#endif
