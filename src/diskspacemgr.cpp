#include "diskspacemgr.h"

//
//
//
DiskSpaceMgr::DiskSpaceMgr( UnixFile& uf )
    : m_uf( uf )
    , m_curr( 0 )
{

}

//
//
//
Page DiskSpaceMgr::Read( PageId id ) const
{
    Page page;
    page.Read( m_uf, id );
    return page;
}

//
//
//
void DiskSpaceMgr::Write( const Page& page, PageId id ) const
{
    page.Write( m_uf , id );
}

//
//
//
PageId DiskSpaceMgr::AllocatePage()
{
    m_curr++;

    m_uf.Allocate( Page::PageSize );
    return ( m_curr - 1 );
}
