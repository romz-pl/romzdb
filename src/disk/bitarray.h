#ifndef ROMZDB_DISK_BITARRAY_H
#define ROMZDB_DISK_BITARRAY_H

#include <cstdint>
#include <climits>
#include <cassert>

template< std::uint32_t SIZE >
class BitArray
{
public:
    BitArray();
    void set( std::uint32_t k );
    void reset( std::uint32_t k );
    void flip( std::uint32_t k );
    bool test( std::uint32_t k ) const;

    std::uint32_t* data();
    const std::uint32_t* data() const;

private:
    std::uint32_t idx( std::uint32_t k ) const;
    std::uint32_t mask( std::uint32_t k ) const;

private:
    // Number of bits in std::uint32_t type
    static constexpr std::uint32_t m_bits_no = CHAR_BIT * sizeof( std::uint32_t );

    std::uint32_t m_array[ ( SIZE + m_bits_no - 1 ) / m_bits_no ];


};

//
//
//
template< std::uint32_t SIZE >
BitArray< SIZE >::BitArray()
    : m_array() // zero initialized
{

}

//
//
//
template< std::uint32_t SIZE >
std::uint32_t BitArray< SIZE >::idx( std::uint32_t k ) const
{
    assert( k < SIZE );
    return k / m_bits_no;
}

//
//
//
template< std::uint32_t SIZE >
std::uint32_t BitArray< SIZE >::mask( std::uint32_t k ) const
{
    assert( k < SIZE );
    return 1U << ( k % m_bits_no );
}

//
// Sets k-th bit to ON
//
template< std::uint32_t SIZE >
void BitArray< SIZE >::set( std::uint32_t k )
{
    m_array[ idx( k ) ] |= mask( k );
}

//
// Sets k-th bit to OFF
//
template< std::uint32_t SIZE >
void BitArray< SIZE >::reset( std::uint32_t k )
{
    m_array[ idx( k ) ] &= ~mask( k );
}

//
// Returns "true" if k-th bit is ON
//
template< std::uint32_t SIZE >
bool BitArray< SIZE >::test( std::uint32_t k ) const
{
  return ( ( m_array[ idx( k ) ] & mask( k ) ) != 0 );
}

//
// Flips k-th bit
//
template< std::uint32_t SIZE >
void BitArray< SIZE >::flip( std::uint32_t k )
{
    m_array[ idx( k ) ] ^= mask( k );
}

//
//
//
template< std::uint32_t SIZE >
std::uint32_t* BitArray< SIZE >::data()
{
    return m_array;
}

//
//
//
template< std::uint32_t SIZE >
const std::uint32_t* BitArray< SIZE >::data() const
{
    return m_array;
}


#endif
