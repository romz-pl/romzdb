#include "pageoffset.h"
#include <limits>
#include <cassert>

//
//
//
const std::uint16_t PageOffset::m_invalid = std::numeric_limits< std::uint16_t >::max();

//
//
//
PageOffset::PageOffset( std::uint16_t value )
    : m_value( value )
{

}

//
//
//
std::uint16_t PageOffset::GetValue() const
{
    return m_value;
}

//
//
//
PageOffset PageOffset::operator += ( const PageOffset& v )
{
    assert( IsValid() );
    m_value += v.m_value;
    return *this;
}

//
//
//
PageOffset PageOffset::operator -= ( const PageOffset& v )
{
    assert( IsValid() );
    m_value -= v.m_value;
    return *this;
}

//
//
//
bool PageOffset::operator == ( const PageOffset& v ) const
{
    return ( m_value == v.m_value );
}

//
//
//
bool PageOffset::operator < ( const PageOffset& v ) const
{
    return ( m_value < v.m_value );
}

//
//
//
void PageOffset::SetInvalid()
{
    m_value = m_invalid;
}

//
//
//
bool PageOffset::IsValid() const
{
    return ( m_value != m_invalid );
}
