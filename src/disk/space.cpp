#include <cassert>
#include "space.h"

//
//
//
Space::Space( const std::string& path, UnixFile::Mode mode )
    : m_dbFile( path, mode )
{

}

//
//
//
DiskBlock Space::Read( PageId pageId ) const
{
    std::pair< const DbFile*, BlockId > ret = Map( pageId );
    const DbFile *dbFile = ret.first;
    const BlockId blockId = ret.second;

    return dbFile->Read( blockId );
}

//
//
//
void Space::Write( const DiskBlock &block, PageId pageId ) const
{
    std::pair< const DbFile*, BlockId > ret = Map( pageId );
    const DbFile *dbFile = ret.first;
    const BlockId blockId = ret.second;
    dbFile->Write( block, blockId );
}

//
//
//
PageId Space::Alloc()
{
    const BlockId blockId = m_dbFile.Alloc( );
    return PageId( blockId.GetValue() );
}

//
//
//
void Space::Dealloc( PageId pageId )
{
//    std::pair< const DbFile*, BlockId > ret = Map( pageId );
//    const DbFile *dbFile = ret.first;
//    const BlockId blockId = ret.second;
//    dbFile->Dealloc( blockId );
    const BlockId blockId( pageId.GetValue() );
    m_dbFile.Dealloc( blockId );
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
std::pair< const DbFile *, BlockId > Space::Map( PageId pageId ) const
{
    assert( pageId.IsValid() );
    return std::make_pair( &m_dbFile, BlockId( pageId.GetValue() ) );
}

