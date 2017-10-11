#include "slot.h"
#include <limits>

const PageOffset Slot::m_freeSlot = std::numeric_limits< PageOffset >::max();

//
//
//
Slot::Slot( PageOffset offset, PageOffset length )
    : m_offset( offset )
    , m_length( length )
{

}

//
//
//
bool Slot::IsFree() const
{
    return ( m_offset == m_freeSlot );
}

//
//
//
void Slot::SetFree()
{
    m_offset = m_freeSlot;
}
