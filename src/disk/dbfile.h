#ifndef ROMZDB_DISK_DBFILE_H
#define ROMZDB_DISK_DBFILE_H


#include "unixfile.h"
#include "blockid.h"
#include "diskblock.h"
#include "bitarray.h"

class DbFile
{
public:
    DbFile( const std::string& path, UnixFile::Mode mode );
    ~DbFile();

    DiskBlock Read( BlockId blockId ) const;
    void Write( const DiskBlock& block, BlockId blockId ) const;

    BlockId Alloc();
    void Dealloc( BlockId blockId );

private:
    UnixFile m_uf;

    BitArray m_spaceMap;

};


#endif
