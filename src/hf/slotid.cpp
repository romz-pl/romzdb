#include "slotid.h"
#include "buffer/page.h"
#include <limits>



//
//
//
SlotId::SlotId( value_type value )
    : m_value( value )
{
    static_assert( std::numeric_limits< value_type >::max() > Page::Size,
                   "The size of SlotId type is not sufficiently large" );
}

//
//
//
bool SlotId::operator==( const SlotId& slotId ) const
{
    return m_value == slotId.m_value;
}

//
//
//
bool SlotId::operator<( const SlotId& slotId ) const
{
    return m_value < slotId.m_value;
}
