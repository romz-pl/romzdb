#ifndef ROMZDB_DIR_H
#define ROMZDB_DIR_H

#include "disk/pageid.h"
#include "dirpage.h"
#include "record.h"
#include "recordid.h"


class HeapFile
{
public:
    explicit HeapFile( BufferMgr& buffer );
    HeapFile( BufferMgr& buffer, PageId header );
    ~HeapFile() = default;

    PageId insert( std::uint32_t count );
    void remove( PageId page_id, std::uint32_t count );

    PageId get_header_page() const;

// private:
    PageId alloc_page( );
    void dispose_page( PageId page_id );

    std::uint64_t get_page_no() const;

private:
    void InsertHeapPage();

private:
    BufferMgr& m_buffer;

    DirPage m_header;

};

#endif
