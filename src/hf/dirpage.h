#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include <optional>
#include "buffer/buffermgr.h"



class DirPage
{
public:
    explicit DirPage( DiskBlock* block );
    ~DirPage();



    PageId get_next_page() const;
    void set_next_page( PageId id );

    std::optional< PageId > insert_record(std::uint32_t count );
    bool remove_record( PageId page_id, std::uint32_t space );

    bool add_page( PageId page_id );
    bool free_page( PageId page_id );

    void init( );

    bool is_next_page() const;

private:
    std::uint32_t max_slot_no() const;

private:
    enum Offset
    {
        Next_page = 0,
        Array = Next_page + sizeof( PageId )
    };

private:
    DiskBlock* m_block;
};

#endif
