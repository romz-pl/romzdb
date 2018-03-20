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
    DiskBlock block;
    block.Read( m_uf, pageId );
    return block;
}

//
//
//
void Space::Write( const DiskBlock &block, PageId pageId ) const
{
    block.Write( m_uf, pageId );
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

