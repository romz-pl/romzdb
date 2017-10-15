#include "heappage.h"
#include <cassert>
#include <cstring>
#include <algorithm>

//
//
//
HeapPage::HeapPage(BufferMgr& bufferMgr, PageId pageId )
    : Page( bufferMgr, pageId, true )
{
    FromPage( );
}

//
//
//
HeapPage::~HeapPage()
{
}

//
//
//
Record HeapPage::Get( SlotId slotIdEx )
{
    const std::uint16_t slotId = slotIdEx.GetValue();
    if( slotId >= m_slot.size() )
    {
        throw std::runtime_error( "HeapPageHdr::Get. Slot does not exist." );
    }

    const Slot& slot = m_slot[ slotId ];

    const char* p = GetData() + slot.m_offset.GetValue();
    Record rec( p, slot.m_length.GetValue() );
    return rec;

}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    const PageOffset recLength = rec.GetLength();
    if( GetFreeSpace() < recLength )
    {
        throw std::runtime_error( "HeapPageHdr::Insert: Not enought space" );
    }

    PageOffset offset( 0 );
    for( Slot& s : m_slot )
        offset += s.m_length;

    m_slot.push_back( Slot( offset, recLength ) );

    char* p = GetData() + offset.GetValue();
    rec.ToPage( p );

    ToPage();
    return SlotId( m_slot.size() - 1 );
}

//
//
//
PageOffset HeapPage::Delete( SlotId slotIdEx )
{
    std::uint16_t slotId = slotIdEx.GetValue();
    if( slotId >= m_slot.size() )
    {
        throw std::runtime_error( "HeapPageHdr::Delete. Slot '" + std::to_string( slotId ) + "' does not exist." );
    }

    const auto length = m_slot[ slotId ].m_length;
    auto it = m_slot.erase( m_slot.begin() + slotId );


    auto fun = [ length ]( Slot& s ){ s.m_offset -= length; };
    std::for_each( it, m_slot.end(), fun );

/*    while( slotId < m_slot.size() )
    {
        m_slot[ slotId ].m_offset -= length;
        slotId++;
    }*/

    ToPage();
    return PageOffset( GetFreeSpace() );
}

//
//
//
std::size_t HeapPage::GetRecordNo() const
{
    return m_slot.size();
}

//
// Returns free space on the Heap File
//
PageOffset HeapPage::GetFreeSpace() const
{
    PageOffset ret( Page::Size );
    for( const Slot& s : m_slot )
    {
        ret -= s.m_length;
        ret -= PageOffset( sizeof( s.m_length ) );
        ret -= PageOffset( sizeof( s.m_offset ) );
    }

    ret -= PageOffset( sizeof( m_slot.size() ) );

    return ret;

}

//
//
//
void HeapPage::ToPage( )
{
    char* p = GetData() + Page::Size;

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
void HeapPage::FromPage()
{
    const char* p = GetData() + Page::Size;

    std::size_t size = 0;
    p -= sizeof( size );
    std::memcpy( &size, p, sizeof( size ) );

    for( std::size_t i = 0; i < size; i++ )
    {
        const Slot slot = Slot::FromPage( p );
        m_slot.push_back( slot );
    }
}
