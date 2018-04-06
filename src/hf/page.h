#ifndef ROMZDB_PAGE_H
#define ROMZDB_PAGE_H

#include "buffer/buffermgr.h"


class Page
{
public:
    explicit Page( BufferMgr& buffer );
    Page( BufferMgr& buffer, PageId page_id );
    ~Page();

    PageId get_page_id( ) const;


protected:
    BufferMgr& m_buffer;

    /*const*/ PageId m_page_id;

    DiskBlock* m_block;

    bool m_dirty;
};

#endif
