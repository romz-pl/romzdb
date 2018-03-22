#ifndef ROMZDB_DISK_BITARRAY_H
#define ROMZDB_DISK_BITARRAY_H

#include <cstdint>
#include <climits>
#include <cassert>
#include <vector>

class BitArray
{
public:
    BitArray( std::uint32_t bit_no );
    ~BitArray() = default;


    void set( std::uint32_t k );
    void reset( std::uint32_t k );
    void flip( std::uint32_t k );
    bool test( std::uint32_t k ) const;

    void* data();
    const void* data() const;

    std::uint32_t bit_no() const;
    std::uint32_t byte_no() const;

private:
    std::uint32_t idx( std::uint32_t k ) const;
    std::uint32_t mask( std::uint32_t k ) const;

private:
    // Number of bits in std::uint32_t type
    static constexpr std::uint32_t m_width = CHAR_BIT * sizeof( std::uint32_t );

    // Number of bits
    const std::uint32_t m_bit_no;

    std::vector< std::uint32_t > m_array;// [ ( SIZE + m_bits_no - 1 ) / m_bits_no ];


};


#endif
