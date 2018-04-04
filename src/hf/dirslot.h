#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include "disk/pageid.h"
//#include "pageoffset.h"
//#include "record.h"

class DirSlot
{
public:
    // DirSlot();
    // ~DirSlot() = default;

    bool insert_record( std::uint32_t count );
    bool remove_record( PageId page_id, std::uint32_t count );

    bool alloc_page( PageId page_id );
    bool dispose_page( PageId page_id );

    void make_invalid();

    PageId get_page_id() const;

    bool is_empty() const;

private:
    bool m_valid;

    // Page into the "DirSlot" points
    PageId m_page_id;

    // Free space on the "m_pageId" page
    std::uint32_t m_free_space;

    static const std::uint32_t m_max_free_space;
};

#endif
