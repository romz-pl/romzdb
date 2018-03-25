#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include "disk/pageid.h"
//#include "pageoffset.h"
//#include "record.h"

class DirSlot
{
public:
    DirSlot( PageId page_id, std::uint32_t free_space );

    bool is_free( std::uint32_t free_space ) const;
    bool is_empty( ) const;

    void empty();

public:
    // Page into the "DirSlot" points
    PageId m_page_id;

    // Free space on the "m_pageId" page
    std::uint32_t m_free_space;
};

#endif
