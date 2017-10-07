#include "diskspacemgr.h"

//
//
//
DiskSpaceMgr::DiskSpaceMgr( UnixFile& uf )
    : m_uf( uf )
{

}

//
//
//
Page DiskSpaceMgr::Read( PageId id )
{
    Page page;
    page.Read( m_uf, id );
    return page;
}

//
//
//
void DiskSpaceMgr::Write( const Page& page, PageId id )
{
    page.Write( m_uf , id );
}
