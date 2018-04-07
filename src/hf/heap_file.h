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

    RecordId insert( const Record& rec );
    void remove( RecordId record_id );
    Record get( RecordId record_id );

    std::uint32_t get_record_no() const;

    PageId get_header_page() const;

private:
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
