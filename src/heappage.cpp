#include "heappage.h"
#include <cassert>
#include <cstring>

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
Record HeapPage::Get( SlotId slotId )
{
    const Slot& slot = m_slot[ slotId ];

    const char* p = GetData() + slot.m_offset;
    Record rec( p, slot.m_length );
    return rec;

}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    const std::size_t recLength = rec.GetLength();
    if( GetFreeSpace() < static_cast< std::int32_t >( recLength ) )
    {
        throw std::runtime_error( "HeapPageHdr::Insert: Not enought space" );
    }

    PageOffset offset = 0;
    for( Slot& s : m_slot )
        offset += s.m_length;

    m_slot.push_back( Slot( offset, recLength ) );

    char* p = GetData() + offset;
    rec.ToPage( p );

    ToPage();
    return ( m_slot.size() - 1 );
}

//
//
//
PageOffset HeapPage::Delete( SlotId slotId )
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

    ToPage();
    return GetFreeSpace();
}

//
//
//
std::size_t HeapPage::GetRecordNo() const
{
    return m_slot.size();
}

//
// Returns free space on the Heap File managed by this Header
//
std::int32_t HeapPage::GetFreeSpace() const
{
    std::int32_t ret = Page::Size;
    for( const Slot& s : m_slot )
    {
        ret -= s.m_length;
        ret -= sizeof( s.m_length );
        ret -= sizeof( s.m_offset );
    }

    ret -= sizeof( m_slot.size() );

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
