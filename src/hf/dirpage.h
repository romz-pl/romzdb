#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include <optional>
#include "buffer/buffermgr.h"
#include "dirslot.h"


class DirPage
{
public:
    explicit DirPage( BufferMgr& buffer );
    DirPage( BufferMgr& buffer, PageId page_id );
    ~DirPage();



    PageId get_next_page() const;
    void set_next_page( PageId id );

    std::optional< PageId > insert_record(std::uint32_t count );
    bool remove_record( PageId page_id, std::uint32_t space );

    bool alloc_page( PageId page_id );
    bool dispose_page( PageId page_id );


    bool is_next_page() const;

    PageId get_page_id( ) const;

private:
    std::uint32_t max_slot_no() const;

    void init( );
    DirSlot* get_slot();

private:
    enum Offset
    {
        Next_page = 0,
        Slot = Next_page + sizeof( PageId )
    };

private:
    BufferMgr& m_buffer;

    /*const*/ PageId m_page_id;

    DiskBlock* m_block;

    bool m_dirty;

    static const PageId m_invalid_page_id;
};

#endif
