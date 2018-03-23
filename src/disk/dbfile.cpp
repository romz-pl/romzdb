#include <cassert>
#include <stdexcept>
#include <climits>
#include "dbfile.h"
#include "div_ceil.h"

//
// Open Db file
//
DbFile::DbFile( const std::string& path )
    : m_uf( path, UnixFile::Mode::Open )
    , m_spaceMap( m_uf )
{
}

//
// Create Db file
//
DbFile::DbFile( const std::string& path, std::uint32_t max_size )
    : m_uf( path, UnixFile::Mode::Create )
    , m_spaceMap( m_uf, max_size )
{
}

//
//
//
DbFile::~DbFile()
{

}

//
//
//
DiskBlock DbFile::Read( BlockId blockId ) const
{
    if( !m_spaceMap.is_allocated( blockId ) )
    {
        throw std::runtime_error( "DbFile::Read: Block not in file" );
    }

    DiskBlock block;
    block.Read( m_uf, blockId );
    return block;
}

//
//
//
void DbFile::Write( const DiskBlock& block, BlockId blockId ) const
{
    if( !m_spaceMap.is_allocated( blockId ) )
    {
        throw std::runtime_error( "DbFile::Write: Block not in file" );
    }

    block.Write( m_uf, blockId );
}

//
//
//
BlockId DbFile::Alloc()
{
    const BlockId blockId = m_spaceMap.allocate();
    DiskBlock().Write( m_uf, blockId );
    return blockId;
}

//
//
//
void DbFile::Dealloc( BlockId blockId )
{
    m_spaceMap.free( blockId );
}

