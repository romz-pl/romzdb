#include "heappagehdr.h"
#include <cassert>

//
//
//
void HeapPageHdr::ToPage( Page& page ) const
{
    char* p = page.GetData() + Page::PageSize;

    std::size_t size = m_slot.size();
    p -= sizeof( size );
    std::memcpy( p, &size, sizeof( size ) );

    for( const Slot& s : m_slot )
    {
        s.ToPage( p );
    }
}

//
//
//
void HeapPageHdr::FromPage( const Page& page )
{
    const char* p = page.GetData() + Page::PageSize;

    std::size_t size = 0;
    p -= sizeof( size );
    std::memcpy( &size, p, sizeof( size ) );

    for( std::size_t i = 0; i < size; i++ )
    {
        const Slot slot = Slot::FromPage( p );
        m_slot.push_back( slot );
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
    if( slotId >= m_slot.size() )
    {
        throw std::runtime_error( "HeapPageHdr::GetSlot. Slot '" + std::to_string( slotId ) + "' does not exist." );
    }
    return m_slot[ slotId ];
}

//
// Inserts slots for tracking Record of recLength.
// Returns offset, where the record can be inserted
//
std::pair< PageOffset, SlotId > HeapPageHdr::Insert( std::size_t recLength )
{
    if( GetFreeSpace() < static_cast< std::int32_t >( recLength ) )
    {
        throw std::runtime_error( "HeapPageHdr::Insert: Not enought space" );
    }

    PageOffset offset = 0;
    for( Slot& s : m_slot )
        offset += s.m_length;

    m_slot.push_back( Slot( offset, recLength ) );
    return std::make_pair( offset, m_slot.size() - 1 );
}

//
// Deletes the slots.
// The array of slots is updated
//
PageOffset HeapPageHdr::Delete( SlotId slotId )
{
    if( slotId >= m_slot.size() )
    {
        throw std::runtime_error( "HeapPageHdr::Delete. Slot '" + std::to_string( slotId ) + "' does not exist." );
    }

    const auto length = m_slot[ slotId ].m_length;
    m_slot.erase( m_slot.begin() + slotId );

    while( slotId < m_slot.size() )
    {
        m_slot[ slotId ].m_offset -= length;
        slotId++;
    }

    return GetFreeSpace();
}

//
// Returns free space on the Heap File managed by this Header
//
std::int32_t HeapPageHdr::GetFreeSpace() const
{
    std::int32_t ret = Page::PageSize;
    for( const Slot& s : m_slot )
    {
        ret -= s.m_length;
        ret -= sizeof( s.m_length );
        ret -= sizeof( s.m_offset );
    }

    ret -= sizeof( m_slot.size() );

    return ret;

}
