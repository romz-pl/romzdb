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
    CheckSlotId( slotIdEx );

    const std::uint16_t slotId = slotIdEx.GetValue();
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

    // New record is alway inserted at he end of current records
    PageOffset offset( 0 );
    for( Slot& s : m_slot )
        offset += s.m_length;

    auto pred = []( const Slot& s ){ return s.IsValid(); };
    auto it = std::find_if_not( m_slot.begin(), m_slot.end(), pred );
    SlotId ret( 0 );
    if( it == m_slot.end() )
    {
        m_slot.push_back( Slot( offset, recLength ) );
        ret = SlotId( m_slot.size() - 1 );
    }
    else
    {
        *it = Slot( offset, recLength );
        ret = SlotId( it - m_slot.begin() );
    }

    char* p = GetData() + offset.GetValue();
    rec.ToPage( p );

    ToPage();
    return ret;
}

//
//
//
PageOffset HeapPage::Delete( SlotId slotIdEx )
{
    CheckSlotId( slotIdEx );

    std::uint16_t slotId = slotIdEx.GetValue();

    const auto length = m_slot[ slotId ].m_length;
    m_slot[ slotId ].SetInvalid();

    // Move records to keep them compactly stored (without holes)
    auto it = m_slot.begin() + slotId + 1;
    for( ; it < m_slot.end(); ++it )
    {
        if( it->IsValid() )
        {
            it->m_offset -= length;
        }
    }


    ToPage();
    return PageOffset( GetFreeSpace() );
}

//
//
//
void HeapPage::CheckSlotId( SlotId slotIdEx ) const
{
    const std::uint16_t slotId = slotIdEx.GetValue();

    if( slotId >= m_slot.size() )
    {
        throw std::runtime_error( "HeapPage::CheckSlotId: Slot '" + std::to_string( slotId ) + "' does not exist." );
    }

    if( !m_slot[ slotId ].IsValid() )
    {
        throw std::runtime_error( "HeapPage::CheckSlotId: Invalid slot." );
    }
}

//
//
//
std::size_t HeapPage::GetRecordNo() const
{
    auto pred = []( const Slot& s ){ return s.IsValid(); };
    return std::count_if( m_slot.begin(), m_slot.end(), pred );
}

//
// Returns free space on the Heap File
//
PageOffset HeapPage::GetFreeSpace() const
{
    std::int32_t ret = Page::Size;
    for( const Slot& s : m_slot )
    {
        ret -= s.m_length.GetValue();
        ret -= sizeof( s.m_length );
        ret -= sizeof( s.m_offset );
    }

    ret -= sizeof( m_slot.size() );

    assert( ret >= 0 );
    return PageOffset( ret );

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

    m_bufferMgr.MarkDirty( m_pageId );
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
