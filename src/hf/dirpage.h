#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include <optional>
#include "buffer/buffermgr.h"



class DirPage
{
public:
    DirPage( DiskBlock* block );
    ~DirPage();

    std::optional< PageId > find( std::uint32_t free_space );
    void clean( PageId page_id, std::uint32_t space );

    std::uint32_t get_slot_no() const;
    void set_slot_no( std::uint32_t v ) const;

    std::uint32_t max_slot_no() const;

    PageId get_next_page() const;
    void set_next_page( PageId id );

    bool add( PageId page_id, std::uint32_t free_space );
    bool remove( PageId page_id );

    void init( );

private:
    enum Offset
    {
        Slot_no = 0,
        Next_page = Slot_no + sizeof( PageId ),
        Array = Next_page + sizeof( std::uint32_t )
    };

private:
    DiskBlock* m_block;
};

#endif
