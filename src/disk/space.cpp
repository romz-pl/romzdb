#include "space.h"

//
//
//
Space::Space( const std::string& path, UnixFile::Mode mode )
    : m_uf( path, mode )
    , m_nextPage( 0 )
{

}

//
//
//
DiskBlock Space::Read( PageId pageId ) const
{
    const off_t offset = PageIdToOffset( pageId );
    DiskBlock block;
    m_uf.Read( block.GetData(), DiskBlock::Size, offset );
    return block;
}

//
//
//
void Space::Write( const DiskBlock &block, PageId pageId ) const
{
    const off_t offset = PageIdToOffset( pageId );
    m_uf.Write( block.GetData(), DiskBlock::Size, offset );
}

//
//
//
PageId Space::Allocate()
{
    m_nextPage++;

    m_uf.Allocate( DiskBlock::Size );
    return PageId( m_nextPage - 1 );
}

//
//
//
off_t Space::PageIdToOffset( PageId pageId ) const
{
    return pageId.GetValue() * static_cast< off_t >( DiskBlock::Size );
}
