#ifndef ROMZDB_DISK_SPACE_H
#define ROMZDB_DISK_SPACE_H

#include "unixfile.h"
#include "pageid.h"
#include "diskblock.h"

class Space
{
public:
    explicit Space( const std::string& path, UnixFile::Mode mode );
    ~Space() = default;

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId Allocate();

private:
    off_t PageIdToOffset( PageId pageId ) const;


private:
    UnixFile m_uf;

    // Next unallocated page
    std::size_t m_nextPage;

};

#endif
