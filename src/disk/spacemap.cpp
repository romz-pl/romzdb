#include <cassert>
#include <stdexcept>
#include <climits>

#include "spacemap.h"
#include "diskblock.h"
#include "div_ceil.h"

//
//
//
SpaceMap::SpaceMap( UnixFile& uf )
    : m_uf( uf )
    , m_bitMap( nullptr )
{
    open();
}

//
// max_size - maximal size of file in bytes
//
SpaceMap::SpaceMap( UnixFile& uf, std::uint32_t max_size )
    : m_uf( uf )
    , m_bitMap( nullptr )
{
    create( max_size );
}

//
//
//
SpaceMap::~SpaceMap()
{
    write();
    delete m_bitMap;
}

//
//
//
void SpaceMap::create( std::uint32_t max_size )
{
    // maximal allowed number of bloks in file
    const std::uint32_t max_block_no = max_size / DiskBlock::Size;

    std::uint32_t bit_no = bits_on_block() - bits_in_header();
    m_map_block_no = 1;

    while( true )
    {
        if( bit_no > max_block_no )
            break;

        bit_no += bits_on_block();
        m_map_block_no++;
    }
    m_data_block_no = max_block_no - m_map_block_no;

    m_bitMap = new BitArray( m_map_block_no + m_data_block_no );
}

//
//
//
void SpaceMap::open( )
{
    off_t offset = 0;

    m_uf.Read( &m_version, sizeof( m_version ), offset );
    offset += sizeof( m_version );

    m_uf.Read( &m_map_block_no, sizeof( m_map_block_no ), offset );
    offset += sizeof( m_map_block_no );

    m_uf.Read( &m_data_block_no, sizeof( m_data_block_no ), offset );
    offset += sizeof( m_data_block_no );

    m_bitMap = new BitArray( m_data_block_no + m_map_block_no );
    m_uf.Read( m_bitMap->data(), m_bitMap->byte_no(), offset );
}

//
//
//
void SpaceMap::write( )
{
    off_t offset = 0;

    m_uf.Write( &m_map_block_no, sizeof( m_map_block_no ), offset );
    offset += sizeof( m_map_block_no );

    m_uf.Write( &m_data_block_no, sizeof( m_data_block_no ), offset );
    offset += sizeof( m_data_block_no );

    m_uf.Write( m_bitMap->data(), m_bitMap->byte_no(), offset );
}

//
// Returns number of bits in header.
//
std::uint32_t SpaceMap::bits_in_header() const
{
    std::uint32_t s = 0;
    s += sizeof( m_version );
    s += sizeof( m_map_block_no );
    s += sizeof( m_map_block_no );

    return CHAR_BIT * s;
}

//
//
//
std::uint32_t SpaceMap::bits_on_block() const
{
    return CHAR_BIT * DiskBlock::Size;
}

//
//
//
bool SpaceMap::is_allocated( BlockId blockId ) const
{
    const std::uint32_t v = blockId.GetValue();
    assert( v >= m_map_block_no ); // Blocks reserved for space map
    assert( v < m_bitMap->bit_no() );
    return m_bitMap->test( v );
}

//
//
//
BlockId SpaceMap::allocate()
{
    for( std::uint32_t i = m_map_block_no; i < m_bitMap->bit_no(); i++ )
    {
        if( !m_bitMap->test( i ) )
        {
            m_bitMap->set( i );
            return BlockId( i );
        }
    }
    throw std::runtime_error( "SpaceMap::Alloc: SpaceMap is full." );
}

//
//
//
void SpaceMap::free( BlockId blockId )
{
    if( !is_allocated( blockId ) )
    {
        throw std::runtime_error( "SpaceMap::Dealloc: BlockId was not allocated" );
    }

    m_bitMap->reset( blockId.GetValue() );
}
