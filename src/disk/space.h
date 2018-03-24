#ifndef ROMZDB_DISK_SPACE_H
#define ROMZDB_DISK_SPACE_H

#include <map>
#include "unixfile.h"
#include "pageid.h"
#include "diskblock.h"
#include "blockid.h"
#include "dbfile.h"

class Space
{
public:
    explicit Space( const std::string& path );
    Space( const std::string& path, std::uint32_t max_size );
    ~Space() = default;

    Space( const Space& ) = delete;
    Space& operator=( const Space& ) = delete;

    Space( Space&& ) = delete;
    Space& operator=( Space&& ) = delete;

    //DbFileId Add( DbFile* file );
    //void Remove( DbFileId id );

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId Alloc();
    void Dealloc( PageId pageId );

private:
    std::pair< const DbFile *, BlockId > Map( PageId pageId ) const;

private:
    //std::map< DbFileId, DbFile* > m_file;

    DbFile m_dbFile;
};

#endif
