#ifndef ROMZDB_DISKSPACEMGR_H
#define ROMZDB_DISKSPACEMGR_H

#include "unixfile.h"
#include "pageid.h"
#include "diskblock.h"

class DiskSpaceMgr
{
public:
    DiskSpaceMgr( UnixFile& uf );
    ~DiskSpaceMgr() = default;

    DiskBlock Read( PageId pageId ) const;
    void Write( const DiskBlock& block, PageId pageId ) const;

    PageId AllocatePage();

private:
    off_t PageIdToOffset( PageId pageId ) const;


private:
    UnixFile& m_uf;

    PageId m_curr;

};

#endif
