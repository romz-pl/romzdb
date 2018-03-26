#ifndef ROMZDB_DISK_DBFILE_H
#define ROMZDB_DISK_DBFILE_H


#include "unixfile.h"
#include "blockid.h"
#include "diskblock.h"
#include "spacemap.h"

class DbFile
{
public:
    DbFile( UnixFile& uf, std::uint32_t max_size );
    ~DbFile() = default;

    DbFile( const DbFile& ) = delete;
    DbFile& operator=( const DbFile& ) = delete;

    DbFile( DbFile&& ) = delete;
    DbFile& operator=( DbFile&& ) = delete;

    DiskBlock Read( BlockId blockId ) const;
    void Write( const DiskBlock& block, BlockId blockId ) const;

    BlockId Alloc();
    void Dealloc( BlockId blockId );

    std::uint32_t free_block_no() const;
    bool full() const;

private:
    UnixFile& m_uf;

    SpaceMap m_spaceMap;
};


#endif
