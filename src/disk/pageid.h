#ifndef ROMZDB_DISK_PAGEID_H
#define ROMZDB_DISK_PAGEID_H

//
// Uniquely identifies the logical page.
//

#include "blockid.h"
#include "dbfileid.h"
#include "util/totally_ordered.h"

class PageId : public totally_ordered< PageId >
{
public:
    PageId( std::uint32_t block_id, std::uint32_t db_file_id );

    PageId( BlockId block_id, DbFileId db_file_id );

    PageId( const PageId& ) = default;
    PageId& operator=( const PageId& ) = default;

    PageId( PageId&& ) = default;
    PageId& operator=( PageId&& ) = default;

    BlockId get_block_id() const;
    DbFileId get_db_file_id() const;

    bool operator==( const PageId& v ) const;
    bool operator< ( const PageId& v ) const;

private:
    BlockId m_block_id;

    DbFileId m_db_file_id;


};

#endif
