#ifndef ROMZDB_DISK_DBFILE_H
#define ROMZDB_DISK_DBFILE_H


#include "unixfile.h"
#include "blockid.h"
#include "diskblock.h"
#include "spacemap.h"

class DbFile
{
public:
    DbFile( const std::string& path );
    DbFile( const std::string& path, std::uint32_t max_size );
    ~DbFile();

    DiskBlock Read( BlockId blockId ) const;
    void Write( const DiskBlock& block, BlockId blockId ) const;

    BlockId Alloc();
    void Dealloc( BlockId blockId );

private:


private:
    UnixFile m_uf;

    SpaceMap m_spaceMap;

};


#endif
