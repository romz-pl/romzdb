#ifndef ROMZDB_PAGEOFFSET_H
#define ROMZDB_PAGEOFFSET_H

#include <cstdint>

class PageOffset
{
public:
    PageOffset( std::uint16_t value );
    ~PageOffset() = default;

    std::uint16_t GetValue() const;
    PageOffset operator+=( const PageOffset& v );
    PageOffset operator-=( const PageOffset& v );


public:
    std::uint16_t m_value;
};


#endif
