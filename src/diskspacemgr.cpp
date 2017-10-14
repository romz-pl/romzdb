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
DiskBlock DiskSpaceMgr::Read( PageId pageId ) const
{
    const off_t offset = PageIdToOffset( pageId );
    DiskBlock block;
    m_uf.Read( block.GetData(), DiskBlock::Size, offset );
    return block;
}

//
//
//
void DiskSpaceMgr::Write( const DiskBlock &block, PageId pageId ) const
{
    const off_t offset = PageIdToOffset( pageId );
    m_uf.Write( block.GetData(), DiskBlock::Size, offset );
}

//
//
//
PageId DiskSpaceMgr::AllocatePage()
{
    m_curr++;

    m_uf.Allocate( DiskBlock::Size );
    return ( m_curr - 1 );
}

//
//
//
off_t DiskSpaceMgr::PageIdToOffset( PageId pageId ) const
{
    return pageId * static_cast< off_t >( DiskBlock::Size );
}
