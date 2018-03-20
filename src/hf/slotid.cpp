#include "slotid.h"
#include "page.h"
#include <limits>

static_assert( std::numeric_limits< std::uint16_t >::max() > Page::Size,
               "The size of SlotId type is not sufficiently large" );

//
//
//
SlotId::SlotId( std::uint16_t value )
    : m_value( value )
{

}

//
//
//
std::uint16_t SlotId::GetValue() const
{
    return m_value;
}

//
//
//
bool SlotId::operator ==( const SlotId& slotId ) const
{
    return m_value == slotId.m_value;
}
