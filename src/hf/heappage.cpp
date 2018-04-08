#include <cassert>
#include <cstring>
#include "heappage.h"
#include <iostream>


//
//
//
HeapPage::HeapPage( BufferMgr& buffer, PageId page_id )
    : Page( buffer, page_id )
{

}

//
//
//
std::uint16_t HeapPage::get_free_space() const
{
    const char* p = get_data() + Offset::Free_space;
    std::uint16_t v = *reinterpret_cast< const std::uint16_t* >( p );
    return v;
}

//
//
//
void HeapPage::set_free_space( std::uint16_t v )
{
    char* p = get_data() + Offset::Free_space;
    std::memcpy( p, &v, sizeof( std::uint16_t ) );
    m_dirty = true;
}

//
//
//
std::uint16_t HeapPage::get_slot_no() const
{
    const char* p = get_data() + Offset::Slot_no;
    std::uint16_t v = *reinterpret_cast< const std::uint16_t* >( p );
    return v;
}

//
//
//
void HeapPage::set_slot_no( std::uint16_t v )
{
    char* p = get_data() + Offset::Slot_no;
    std::memcpy( p, &v, sizeof( std::uint16_t ) );
    m_dirty = true;
}

//
//
//
Slot* HeapPage::get_slot_array()
{
    char* p = get_data() + Offset::Slot_array;
    return reinterpret_cast< Slot* >( p );
}

//
//
//
const Slot* HeapPage::get_slot_array() const
{
    const char* p = get_data() + Offset::Slot_array;
    return reinterpret_cast< const Slot* >( p );
}

//
//
//
Slot* HeapPage::get_slot( std::uint16_t slot_id )
{
    // Here must be minus!
    return get_slot_array() - slot_id;
}

//
//
//
const Slot* HeapPage::get_slot( std::uint16_t slot_id ) const
{
    // Here must be minus!
    return get_slot_array() - slot_id;
}

//
//
//
Record HeapPage::Get( SlotId slotIdEx )
{
    CheckSlotId( slotIdEx );

    const std::uint16_t slot_id = slotIdEx.to_uint16();
    return get_slot( slot_id )->get_record( get_data() );
}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    if( GetFreeSpace() < rec.get_length() )
    {
        throw std::runtime_error( "HeapPage::Insert: Not enought space" );
    }

    const std::uint16_t offset = get_free_space();
    const std::uint16_t length = rec.get_length();

    bool found = false;
    std::uint32_t slot_id = 0;
    for( auto it = rbegin(); it != rend(); ++it, slot_id++ )
    {
        if( !it->IsValid() )
        {
            *it = Slot( offset, length );
            found = true;
            break;
        }
    }

    if( !found )
    {
        *( rend() ) = Slot( offset, length );
        set_slot_no( get_slot_no() + 1 );
    }

    char* dest = get_data() + offset;
    rec.copy_to_page( dest );

    set_free_space( offset + length );
    m_dirty = true;

    return SlotId( slot_id );
}

//
//
//
std::uint16_t HeapPage::Remove( SlotId slotIdEx )
{
    CheckSlotId( slotIdEx );

    const std::uint16_t slot_id = slotIdEx.to_uint16();

    Slot* slot = get_slot( slot_id );
    const std::uint16_t length = slot->get_length();
    const std::uint16_t offset = slot->get_offset();

    shift_data( offset, length );

    slot->SetInvalid();

    decrease_slot_offset( offset, length );
    remove_slots();

    m_dirty = true;
    return length;
}

void HeapPage::shift_data( std::uint16_t offset, std::uint16_t length )
{
    char* dest = get_data() + offset;
    char* src = dest + length;
    assert( get_free_space() >= offset + length );
    const std::size_t count = get_free_space() - offset - length;
    std::memmove( dest, src, count );


    set_free_space( get_free_space() - length );
}

//
//
//
void HeapPage::decrease_slot_offset( std::uint16_t offset, std::uint16_t length )
{
    for( auto it = begin(); it != end(); ++it )
    {
        if( it->IsValid() && it->get_offset() > offset )
        {
            it->dec_offset( length );
        }
    }
}

//
// Remove non valid slots if they are at the end of slot array
//
void HeapPage::remove_slots()
{
    for( auto it = begin(); it != end(); ++it )
    {
        if( !it->IsValid()  )
        {
            set_slot_no( get_slot_no() - 1 );
        }
        else
        {
            break;
        }
    }
}

//
//
//
void HeapPage::CheckSlotId( SlotId slotIdEx ) const
{
    const std::uint16_t slotId = slotIdEx.to_uint16();

    if( slotId >= get_slot_no() )
    {
        throw std::runtime_error( "HeapPage::CheckSlotId: Slot '" + std::to_string( slotId ) + "' does not exist." );
    }

    if( !get_slot( slotId )->IsValid() )
    {
        throw std::runtime_error( "HeapPage::CheckSlotId: Invalid slot." );
    }
}

//
//
//
std::uint32_t HeapPage::GetRecordNo() const
{
    std::uint32_t ret = 0;
    for( auto it = begin(); it != end(); ++it )
    {
        if( it->IsValid() )
            ret++;
    }
    return ret;
}


//
// Returns free space on the Heap File
//
std::uint16_t HeapPage::GetFreeSpace() const
{
    std::uint16_t ret = DiskBlock::Size;

    // free space
    ret -= sizeof( std::uint16_t );

    // slot no
    ret -= sizeof( std::uint16_t );

    for( auto it = begin(); it != end(); ++it )
    {
        // Size of the Slot structure
        ret -= sizeof( Slot );

        if( it->IsValid() )
        {
            ret -= it->get_length();
        }
    }
    // For possible new slot
    ret -= sizeof( Slot );

    return ret;
}

//
// Returns maximal allowed record length
//
std::uint32_t HeapPage::GetMaxRecordLength()
{
    std::size_t maxRecordLength = DiskBlock::Size; // This is the size of the page

    // free space
    maxRecordLength -= sizeof( std::uint16_t );

    // slot no
    maxRecordLength -= sizeof( std::uint16_t );

    // For storing record, one slot is required. This is the size of the Slot
    maxRecordLength -= sizeof( Slot );


    return static_cast< std::uint32_t >( maxRecordLength );
}

//
//
//
HeapPage::iterator HeapPage::begin()
{
    return iterator( get_slot_array() - get_slot_no() + 1 );
}

//
//
//
HeapPage::const_iterator HeapPage::begin() const
{
    return const_iterator( get_slot_array() - get_slot_no() + 1 );
}

//
//
//
HeapPage::iterator HeapPage::end()
{
    return iterator( get_slot_array() + 1 );
}

//
//
//
HeapPage::const_iterator HeapPage::end() const
{
    return const_iterator( get_slot_array() + 1 );
}

//
//
//
HeapPage::reverse_iterator HeapPage::rbegin()
{
    return reverse_iterator( end() );
}

//
//
//
HeapPage::const_reverse_iterator HeapPage::rbegin() const
{
    return const_reverse_iterator( end() );
}

//
//
//
HeapPage::reverse_iterator HeapPage::rend()
{
    return reverse_iterator( begin() );
}

//
//
//
HeapPage::const_reverse_iterator HeapPage::rend() const
{
    return const_reverse_iterator( begin() );
}

