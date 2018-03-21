#include <cassert>
#include <stdexcept>
#include "dbfile.h"


//
//
//
DbFile::DbFile( const std::string& path, UnixFile::Mode mode )
    : m_uf( path, mode )
{
    assert( m_spaceMap.byte_no() == DiskBlock::Size );

    if( mode == UnixFile::Mode::Open )
    {
        m_uf.Read( m_spaceMap.data(), m_spaceMap.byte_no(), 0 );
    }
    else if( mode == UnixFile::Mode::Create )
    {
        m_uf.Write( m_spaceMap.data(), m_spaceMap.byte_no(), 0 );
    }
}

//
//
//
DiskBlock DbFile::Read( BlockId blockId ) const
{
    if( !m_spaceMap.test( blockId.GetValue() ) )
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
    if( !m_spaceMap.test( blockId.GetValue() ) )
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
    for( std::uint32_t i = 0; i < m_spaceMap.byte_no(); i++ )
    {
        if( !m_spaceMap.test( i ) )
        {
            m_spaceMap.set( i );
            return BlockId( i );
        }
    }
    throw std::runtime_error( "DbFile::Alloc: File full" );
}

//
//
//
void DbFile::Dealloc( BlockId blockId )
{
    if( !m_spaceMap.test( blockId.GetValue() ) )
    {
        throw std::runtime_error( "DbFile::Dealloc: Page was not allocated" );
    }

    m_spaceMap.reset( blockId.GetValue() );
}

