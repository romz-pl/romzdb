#include "pageoffset.h"

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
    m_value += v.m_value;
    return *this;
}

//
//
//
PageOffset PageOffset::operator -= ( const PageOffset& v )
{
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
