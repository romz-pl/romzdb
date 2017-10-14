#include "heappagehdr.h"
#include <cassert>

//
//
//
HeapPageHdr::HeapPageHdr( BufferMgr& bufferMgr, PageId pageId )
    : m_bufferMgr( bufferMgr )
    , m_pageId( pageId )
{
    FromPage( );
}

//
//
//
Record HeapPageHdr::Get( SlotId slotId )
{
    const Slot& slot = m_slot[ slotId ];

    Page* page = m_bufferMgr.GetPage( m_pageId, true );
    const char* p = page->GetData();
    p += slot.m_offset;
    Record rec( p, slot.m_length );
    m_bufferMgr.UnpinPage( m_pageId );
    return rec;
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
SlotId HeapPageHdr::Insert( const Record& rec )
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

    Page* page = m_bufferMgr.GetPage( m_pageId, true );
    char* p = page->GetData() + offset;
    rec.ToPage( p );
    m_bufferMgr.UnpinPage( m_pageId );

    ToPage();
    return ( m_slot.size() - 1 );
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

    ToPage();
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

//
//
//
void HeapPageHdr::ToPage( ) const
{
    Page* page = m_bufferMgr.GetPage( m_pageId, true );
    char* p = page->GetData() + Page::PageSize;

    std::size_t size = m_slot.size();
    p -= sizeof( size );
    std::memcpy( p, &size, sizeof( size ) );

    for( const Slot& s : m_slot )
    {
        s.ToPage( p );
    }

    m_bufferMgr.UnpinPage( m_pageId );
}

//
//
//
void HeapPageHdr::FromPage()
{
    Page* page = m_bufferMgr.GetPage( m_pageId, true );
    const char* p = page->GetData() + Page::PageSize;

    std::size_t size = 0;
    p -= sizeof( size );
    std::memcpy( &size, p, sizeof( size ) );

    for( std::size_t i = 0; i < size; i++ )
    {
        const Slot slot = Slot::FromPage( p );
        m_slot.push_back( slot );
    }

    m_bufferMgr.UnpinPage( m_pageId );
}

