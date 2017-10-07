#ifndef ROMZDB_DISKSPACEMGR_H
#define ROMZDB_DISKSPACEMGR_H

#include "unixfile.h"
#include "page.h"

class DiskSpaceMgr
{
public:
    DiskSpaceMgr( UnixFile& uf );
    ~DiskSpaceMgr() = default;

    Page Read( PageId id ) const;
    void Write( const Page& page, PageId id ) const;


private:
    UnixFile& m_uf;

};

#endif
