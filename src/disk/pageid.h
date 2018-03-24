#ifndef ROMZDB_DISK_PAGEID_H
#define ROMZDB_DISK_PAGEID_H

//
// Uniquely identifies the logical page.
//

#include "blockid.h"
#include "dbfileid.h"

class PageId
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

    bool IsValid() const;

    bool operator==( const PageId& v ) const;

public:
    static const std::uint32_t m_invalid;

private:
    BlockId m_block_id;

    DbFileId m_db_file_id;


};

#endif
