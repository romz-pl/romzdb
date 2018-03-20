#ifndef ROMZDB_DISK_DISK_H
#define ROMZDB_DISK_DISK_H

#include "unixfile.h"
#include "diskspacemgr.h"


class Disk
{
public:
    Disk( const std::string& path, UnixFile::Mode mode );
    ~Disk() = default;

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId Allocate();

private:
    UnixFile m_uf;

    DiskSpaceMgr m_ds;
};


#endif
