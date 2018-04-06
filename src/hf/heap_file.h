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
    HeapFile( BufferMgr& buffe, PageId header_page_id );
    ~HeapFile() = default;

    void insert( const Record& rec );


    void remove_from_dir( PageId page_id, std::uint32_t count );
    PageId insert_into_dir( std::uint32_t count );

    PageId get_header_page() const;

// private:
    PageId alloc_page( );
    void dispose_page( PageId page_id );

    std::uint64_t get_page_no() const;

private:
    void InsertHeapPage();


private:
    BufferMgr& m_buffer;

    PageId m_header;

};

#endif
