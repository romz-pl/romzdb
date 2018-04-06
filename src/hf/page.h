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

    const char* get_data() const;
    char* get_data();


protected:
    BufferMgr& m_buffer;

    bool m_dirty;

private:

    /*const*/ PageId m_page_id;

    DiskBlock* m_block;
};

#endif
