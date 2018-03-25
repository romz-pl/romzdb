#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include <optional>
#include "buffer/buffermgr.h"



class DirPage
{
public:
    DirPage( DiskBlock* block );
    ~DirPage();

    std::optional< PageId > get( std::uint32_t free_space );
    void clean( PageId page_id, std::uint32_t space );

    std::uint32_t slot_no() const;

    PageId get_next_page() const;
    void set_next_page( PageId id );

private:
    DiskBlock* m_block;
};

#endif
