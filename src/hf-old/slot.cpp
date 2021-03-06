#include "slot.h"
#include <cstring>


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
void Slot::SetInvalid()
{
    m_offset.SetInvalid();
    m_length = PageOffset( 0 );
}

//
//
//
bool Slot::IsValid() const
{
    return m_offset.IsValid();
}

//
//
//
void Slot::ToPage( char *& dest ) const
{
    dest -= sizeof( m_offset );
    std::memcpy( dest, &m_offset, sizeof( m_offset ) );

    dest -= sizeof( m_length );
    std::memcpy( dest, &m_length, sizeof( m_length ) );
}

//
//
//
Slot Slot::FromPage( const char *& src )
{
    PageOffset offset( 0 );
    src -= sizeof( offset );
    std::memcpy( &offset, src, sizeof( offset ) );

    PageOffset length( 0 );
    src -= sizeof( length );
    std::memcpy( &length, src, sizeof( length ) );

    return Slot( offset, length );
}
