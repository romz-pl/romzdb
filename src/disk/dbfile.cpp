#include <cassert>
#include <stdexcept>
#include <climits>
#include "dbfile.h"
#include "util/div_ceil.h"

DbFile::DbFile( UnixFile& uf )
    : m_uf( uf )
    , m_spaceMap( m_uf )
{

}


//
//
//
DbFile::DbFile( UnixFile& uf, std::uint32_t max_size )
    : m_uf( uf )
    , m_spaceMap( m_uf, max_size )
{
}

//
//
//
DiskBlock DbFile::Read( BlockId blockId ) const
{
    if( !m_spaceMap.is_valid( blockId ) )
    {
        throw std::runtime_error( "DbFile::Read: Not valid block." );
    }

    if( !m_spaceMap.is_allocated( blockId ) )
    {
        throw std::runtime_error( "DbFile::Read: Block is not allocated." );
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
    if( !m_spaceMap.is_valid( blockId ) )
    {
        throw std::runtime_error( "DbFile::Write: Not valid block." );
    }

    if( !m_spaceMap.is_allocated( blockId ) )
    {
        throw std::runtime_error( "DbFile::Write: Block is not allocated." );
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
    if( !m_spaceMap.is_valid( blockId ) )
    {
        throw std::runtime_error( "DbFile::Dealloc: Not valid block." );
    }
    m_spaceMap.free( blockId );
}

//
//
//
std::uint32_t DbFile::free_block_no() const
{
    return m_spaceMap.max_data_block_no() - m_spaceMap.curr_data_block_no();
}

//
//
//
bool DbFile::full() const
{
    return ( free_block_no() == 0 );
}

