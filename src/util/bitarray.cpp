#include "bitarray.h"
#include "div_ceil.h"

//
//
//
BitArray::BitArray( std::uint32_t bit_no )
    : m_bit_no( bit_no )
    , m_array( div_ceil( bit_no, m_width ) )
{

}

//
//
//
std::uint32_t BitArray::idx( std::uint32_t k ) const
{
    assert( k < m_bit_no );
    return k / m_width;
}

//
//
//
std::uint32_t BitArray::mask( std::uint32_t k ) const
{
    assert( k < m_bit_no );
    return 1U << ( k % m_width );
}

//
// Sets k-th bit to ON
//
void BitArray::set( std::uint32_t k )
{
    m_array[ idx( k ) ] |= mask( k );
}

//
// Sets k-th bit to OFF
//
void BitArray::reset( std::uint32_t k )
{
    m_array[ idx( k ) ] &= ~mask( k );
}

//
// Returns "true" if k-th bit is ON
//
bool BitArray::test( std::uint32_t k ) const
{
  return ( ( m_array[ idx( k ) ] & mask( k ) ) != 0 );
}

//
// Flips k-th bit
//
void BitArray::flip( std::uint32_t k )
{
    m_array[ idx( k ) ] ^= mask( k );
}

//
//
//
void* BitArray::data()
{
    return m_array.data();
}

//
//
//
const void *BitArray::data() const
{
    return m_array.data();
}

//
//
//
std::uint32_t BitArray::bit_no() const
{
    return m_bit_no;
}

//
//
//
std::uint32_t BitArray::byte_no() const
{
    return m_array.size() * sizeof( std::uint32_t );
}

