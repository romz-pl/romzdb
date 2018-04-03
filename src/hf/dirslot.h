#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include "disk/pageid.h"
//#include "pageoffset.h"
//#include "record.h"

class DirSlot
{
public:
    DirSlot( PageId page_id, std::uint32_t free_space );
    ~DirSlot() = default;

    bool insert_record( std::uint32_t count );
    bool remove_record( PageId page_id, std::uint32_t count );

    bool add_page( PageId page_id );
    bool free_page( PageId page_id );

    void empty();

    PageId get_page_id() const;

private:
    bool is_free( std::uint32_t free_space ) const;
    bool is_empty( ) const;

private:
    // Page into the "DirSlot" points
    PageId m_page_id;

    // Free space on the "m_pageId" page
    std::uint32_t m_free_space;
};

#endif
