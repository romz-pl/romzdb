#include <cstring>
#include <limits>
#include "slot.h"


//
//
//
const std::uint16_t Slot::m_invalid = std::numeric_limits< std::uint16_t >::max();


//
//
//
Slot::Slot( std::uint16_t offset, std::uint16_t length )
    : m_offset( offset )
    , m_length( length )
{
    assert( m_offset != m_invalid );
}


//
//
//
void Slot::SetInvalid()
{
    m_offset = m_invalid;
    m_length = 0;
}

//
//
//
bool Slot::IsValid() const
{
    return ( m_offset != m_invalid );
}

//
//
//
Record Slot::get_record( const char* data ) const
{
    const char* p = data + m_offset;
    return Record( p, m_length );
}

//
//
//
std::uint16_t Slot::get_length() const
{
    assert( IsValid() );
    return m_length;
}

//
//
//
std::uint16_t Slot::get_offset() const
{
    assert( IsValid() );
    return m_offset;
}

//
//
//
void Slot::dec_offset( std::uint16_t v )
{
    assert( m_offset >= v );
    m_offset -= v;
}
