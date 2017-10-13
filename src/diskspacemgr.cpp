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
    static PageId curr = 0;
    curr++;

    Page page;
    page.Write( m_uf , curr );
    return curr;
}
