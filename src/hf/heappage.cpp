#include <cassert>
#include <cstring>
#include "heappage.h"



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
    const std::uint16_t recLength = rec.get_length();
    if( GetFreeSpace() < recLength )
    {
        throw std::runtime_error( "HeapPage::Insert: Not enought space" );
    }

    Slot* begin = get_slot_array();
    Slot* end = begin - get_slot_no();
    Slot* slot = begin;

    bool found = false;
    for( ; slot != end; slot-- )
    {
        if( !slot->IsValid() )
        {
            found = true;
            break;
        }
    }

    const std::uint16_t offset = get_free_space();
    const std::uint16_t length = rec.get_length();

    if( !found )
    {
        set_slot_no( get_slot_no() + 1 );
    }

    *slot = Slot( offset, length );
    const std::uint32_t slot_id = begin - slot;

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

    std::uint16_t slotId = slotIdEx.to_uint16();

    Slot* slot = get_slot( slotId );
    const std::uint16_t length = slot->get_length();
    const std::uint16_t offset = slot->get_offset();

    char* dest = get_data() + offset;
    char* src = dest + length;
    assert( get_free_space() >= offset + length );
    const std::size_t count = get_free_space() - offset - length;
    std::memmove( dest, src, count );


    set_free_space( get_free_space() - length );
    slot->SetInvalid();

    Slot* begin = get_slot_array();
    Slot* end = begin - get_slot_no();
    slot = begin;

    for( ; slot != end; slot-- )
    {
        if( slot->IsValid() && slot->get_offset() > offset )
        {
            slot->dec_offset( length );
        }
    }

    m_dirty = true;
    return length;
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
    const Slot* begin = get_slot_array();
    const Slot* end = begin - get_slot_no();

    std::uint32_t ret = 0;
    for( const Slot* slot = begin; slot != end; slot-- )
    {
        if( slot->IsValid() )
            ret++;
    }
    return ret;
}

//
// Returns free space on the Heap File
//
std::uint16_t HeapPage::GetFreeSpace() const
{
    const Slot* begin = get_slot_array();
    const Slot* slot_end = begin - get_slot_no();

    std::uint16_t ret = DiskBlock::Size;

    // free space
    ret -= sizeof( std::uint16_t );

    // slot no
    ret -= sizeof( std::uint16_t );

    for( const Slot* slot = begin; slot != slot_end; slot-- )
    {
        // Size of the Slot structure
        ret -= sizeof( Slot );

        if( slot->IsValid() )
        {
            ret -= slot->get_length();
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
    maxRecordLength -= 2 * sizeof( Slot );


    return static_cast< std::uint32_t >( maxRecordLength );
}



