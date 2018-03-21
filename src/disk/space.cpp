#include <cassert>
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
    std::pair< const UnixFile*, BlockId > ret = Map( pageId );
    const UnixFile *uf = ret.first;
    const BlockId blockId = ret.second;
    DiskBlock block;
    block.Read( *uf, blockId );
    return block;
}

//
//
//
void Space::Write( const DiskBlock &block, PageId pageId ) const
{
    std::pair< const UnixFile*, BlockId > ret = Map( pageId );
    const UnixFile *uf = ret.first;
    const BlockId blockId = ret.second;
    block.Write( *uf, blockId );
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
// The function maps "pageId" to the pair (UnixFile, BlockId).
//
// The PageId represents the logical page in HeapFile.
// The Space can be represented by an array of UnixFile.
// Each UnixFile is the array of disk blocks identyfied by BlockId.
//
// Current implementation of Space has one UnixFile only, hence the mapping is trivial.
//
std::pair< const UnixFile*, BlockId > Space::Map( PageId pageId ) const
{
    assert( pageId.IsValid() );
    return std::make_pair( &m_uf, BlockId( pageId.GetValue() ) );
}

