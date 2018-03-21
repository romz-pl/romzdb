#ifndef ROMZDB_DISK_SPACE_H
#define ROMZDB_DISK_SPACE_H

#include "unixfile.h"
#include "pageid.h"
#include "diskblock.h"
#include "blockid.h"

class Space
{
public:
    explicit Space( const std::string& path, UnixFile::Mode mode );
    ~Space() = default;

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId Allocate();

private:
    std::pair< const UnixFile *, BlockId> Map( PageId pageId ) const;

private:
    UnixFile m_uf;

    // Next unallocated page
    std::uint32_t m_nextPage;

};

#endif
