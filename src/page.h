#ifndef ROMZDB_PAGE_H
#define ROMZDB_PAGE_H

//
// 1. Represents "the page of data".
//
// 2. The page is piece of memory representing the disk block on the disk
//
// 3. All operations in Database must be done on pages
//

#include "diskblock.h"
#include "pageid.h"
#include "buffermgr.h"



// using PageOffset = std::uint16_t;


class Page
{
public:
    Page( BufferMgr& bufferMgr, PageId pageId, bool multiplePins );
    ~Page();

private:
    BufferMgr& m_bufferMgr;

    const PageId m_pageId;

    // Data stored on the page
    DiskBlock* m_block;
};


#endif
