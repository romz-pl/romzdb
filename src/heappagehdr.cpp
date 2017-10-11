#include "heappagehdr.h"
#include <cassert>

//
//
//
HeapPageHdr::HeapPageHdr()
    : m_freeSpace( 0 )
{

}

//
//
//
void HeapPageHdr::ToPage( Page& page ) const
{
    char* p = page.GetData();

    auto ss = sizeof( m_freeSpace );
    p -= ss;
    std::memcpy( p, &m_freeSpace, ss );

    std::size_t size = m_slot.size();
    ss = sizeof( size );
    p -= ss;
    std::memcpy( p, &size, ss );


    for( const Slot& s : m_slot )
    {
        p -= sizeof( s.m_offset );
        std::memcpy( p, &s.m_offset, ss );

        p -= sizeof( s.m_length );
        std::memcpy( p, &s.m_length, ss );
    }
}

//
//
//
void HeapPageHdr::FromPage( const Page& page )
{
    const char* p = page.GetData();

    auto ss = sizeof( m_freeSpace );
    p -= ss;
    std::memcpy( &m_freeSpace, p, ss );

    std::size_t size = 0;
    ss = sizeof( size );
    p -= ss;
    std::memcpy( &size, p, ss );

    for( std::size_t i = 0; i < size; i++ )
    {
        PageOffset offset;
        p -= sizeof( offset );
        std::memcpy( &offset, p, ss );

        PageOffset length;
        p -= sizeof( length );
        std::memcpy( &length, p, ss );

        m_slot.push_back( Slot( offset, length ) );
    }
}

//
//
//
std::size_t HeapPageHdr::GetSlotNo() const
{
    return m_slot.size();
}

//
//
//
Slot HeapPageHdr::GetSlot( SlotId slotId ) const
{
    assert( slotId < m_slot.size() );
    return m_slot[ slotId ];
}

//
//
//
PageOffset HeapPageHdr::Insert( const Record &rec )
{
    const auto length = rec.GetLength();
    m_slot.push_back( Slot( m_freeSpace, length ) );
    m_freeSpace += length;
    return m_freeSpace;
}

//
//
//
void HeapPageHdr::Delete( SlotId slotId )
{
    assert( slotId < m_slot.size() );

    const auto length = m_slot[ slotId ].m_length;
    m_slot.erase( m_slot.begin() + slotId );

    while( slotId < m_slot.size() )
    {
        m_slot[ slotId ].m_offset -= length;
    }
    m_freeSpace -= length;


}

