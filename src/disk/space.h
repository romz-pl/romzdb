#ifndef ROMZDB_DISK_SPACE_H
#define ROMZDB_DISK_SPACE_H

#include "unixfile.h"
#include "pageid.h"
#include "diskblock.h"
#include "blockid.h"
#include "dbfile.h"

class Space
{
public:
    explicit Space( const std::string& path, UnixFile::Mode mode );
    ~Space() = default;

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId Alloc();
    void Dealloc( PageId pageId );

private:
    std::pair< const DbFile *, BlockId > Map( PageId pageId ) const;

private:
    DbFile m_dbFile;
};

#endif
