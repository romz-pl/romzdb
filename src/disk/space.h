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
    Space();
    explicit Space( DbFile& db_file );
    ~Space() = default;

    Space( const Space& ) = delete;
    Space& operator=( const Space& ) = delete;

    Space( Space&& ) = delete;
    Space& operator=( Space&& ) = delete;

    DbFileId Add( DbFile& file );
    void Remove( DbFileId id );

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId Alloc();
    void Dealloc( PageId pageId );

private:
    DbFile* get_db_file( PageId pageId );
    const DbFile* get_db_file( PageId pageId ) const;

private:
    std::map< DbFileId, DbFile* > m_db_file_map;
};

#endif
