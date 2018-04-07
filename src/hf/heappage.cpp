#include <cassert>
#include <cstring>
#include <algorithm>
#include "heappage.h"



//
//
//
HeapPage::HeapPage( BufferMgr& buffer, PageId page_id )
    : Page( buffer, page_id )
{
    FromPage( );
}



//
//
//
Record HeapPage::Get( SlotId slotIdEx )
{
    CheckSlotId( slotIdEx );

    const std::uint16_t slotId = slotIdEx.to_uint16();
    const Slot& slot = m_slot[ slotId ];

    const char* p = get_data() + slot.m_offset.GetValue();
    Record rec( p, slot.m_length.GetValue() );
    return rec;

}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    const PageOffset recLength = PageOffset( rec.get_length() );
    if( GetFreeSpace() < recLength )
    {
        throw std::runtime_error( "HeapPage::Insert: Not enought space" );
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

    char* p = get_data() + offset.GetValue();
    rec.copy_to_page( p );

    ToPage();
    return ret;
}

//
//
//
std::uint16_t HeapPage::Remove( SlotId slotIdEx )
{
    CheckSlotId( slotIdEx );

    std::uint16_t slotId = slotIdEx.to_uint16();

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
    return length.GetValue();
}

//
//
//
void HeapPage::CheckSlotId( SlotId slotIdEx ) const
{
    const std::uint16_t slotId = slotIdEx.to_uint16();

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
std::uint32_t HeapPage::GetRecordNo() const
{
    auto pred = []( const Slot& s ){ return s.IsValid(); };
    const std::size_t v = std::count_if( m_slot.begin(), m_slot.end(), pred );
    return static_cast< std::uint32_t >( v );
}

//
// Returns free space on the Heap File
//
PageOffset HeapPage::GetFreeSpace() const
{
    std::int32_t ret = DiskBlock::Size;
    for( const Slot& s : m_slot )
    {
        // Size of the record
        ret -= s.m_length.GetValue();

        // Size of the Slot structure
        ret -= sizeof( s );
    }

    // Size of varaiable storing number of slots
    ret -= sizeof( m_slot.size() );

    assert( ret >= 0 );
    return PageOffset( ret );

}

//
//
//
void HeapPage::ToPage( )
{
    char* p = get_data() + DiskBlock::Size;

    std::size_t size = m_slot.size();
    p -= sizeof( size );
    std::memcpy( p, &size, sizeof( size ) );

    for( const Slot& s : m_slot )
    {
        s.ToPage( p );
    }
    m_dirty = true;
}

//
//
//
void HeapPage::FromPage()
{
    const char* p = get_data() + DiskBlock::Size;

    std::size_t size = 0;
    p -= sizeof( size );
    std::memcpy( &size, p, sizeof( size ) );

    for( std::size_t i = 0; i < size; i++ )
    {
        const Slot slot = Slot::FromPage( p );
        m_slot.push_back( slot );
    }
}

/*
//
//
//
void HeapPage::GetRid( std::vector< RecordId >& rid ) const
{
    for( std::size_t i = 0; i < m_slot.size(); i++ )
    {
        if( m_slot[ i ].IsValid() )
        {
            rid.push_back( RecordId( get_page_id(), SlotId( i ) ) );
        }
    }
}
*/


//
// Returns maximal allowed record length
//
std::uint32_t HeapPage::GetMaxRecordLength()
{
    // For details of page format, see the function HeapPage::ToPage

    std::size_t maxRecordLength = DiskBlock::Size; // This is the size of the page

    // For storing record, one slot is required. This is the size of the Slot
    maxRecordLength -= sizeof( Slot );

    // On the page, always number of records are stored
    maxRecordLength -= sizeof( std::size_t );

    return static_cast< std::uint32_t >( maxRecordLength );
}



