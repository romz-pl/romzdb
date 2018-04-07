#include <limits>
#include <stdexcept>
#include "dirslot.h"
#include "disk/diskblock.h"
#include "heappage.h"


const std::uint32_t DirSlot::m_max_free_space = DiskBlock::Size;


/*
//
//
//
DirSlot::DirSlot(  )
    : m_valid( false )
    , m_page_id( 0, 0 )
    , m_free_space( 0 )
{

}
*/


//
//
//
void DirSlot::make_invalid()
{
    m_valid = false;
}

//
//
//
bool DirSlot::is_valid() const
{
    return m_valid;
}

//
//
//
std::optional< RecordId > DirSlot::insert_record( BufferMgr& buffer, const Record& rec )
{
    if( !m_valid )
    {
        return std::nullopt;
    }

    if( m_free_space >= rec.get_length() + sizeof( Slot ) )
    {
        HeapPage hp( buffer, m_page_id );
        const SlotId slot_id = hp.Insert( rec );
        m_free_space = hp.GetFreeSpace().GetValue();
        return RecordId( m_page_id, slot_id );
    }

    return std::nullopt;
}

//
//
//
bool DirSlot::remove_record( BufferMgr& buffer, RecordId record_id )
{
    if( !m_valid )
    {
        return false;
    }

    if( m_page_id != record_id.get_page_id() )
    {
        return false;
    }

    HeapPage hp( buffer, record_id.get_page_id() );
    const std::uint16_t count = hp.Remove( record_id.get_slot_id() );

    const std::uint32_t space_after_remove = m_free_space + count;

    if( space_after_remove > m_max_free_space )
    {
        throw std::runtime_error( "DirSlot::remove_record: More space removed than allocated" );
    }

    m_free_space = space_after_remove;
    return true;
}

//
//
//
bool DirSlot::alloc_page( PageId page_id )
{
    if( !m_valid )
    {
        m_page_id = page_id;
        m_free_space = m_max_free_space;
        m_valid = true;
        return true;
    }

    return false;
}

//
//
//
bool DirSlot::dispose_page( PageId page_id )
{
    if( !m_valid )
    {
        return false;
    }

    if( !is_empty() )
    {
        throw std::runtime_error( "DirSlot::dispose_page: The page is not empty" );
    }

    if( m_page_id != page_id )
    {
        return false;
    }

    m_valid = false;
    return true;

}

//
//
//
std::uint32_t DirSlot::get_record_no( BufferMgr& buffer ) const
{
    if( !m_valid )
    {
        return 0;
    }

    if( is_empty() )
    {
        return 0;
    }

    HeapPage hp( buffer, m_page_id );
    return hp.GetRecordNo();
}

//
//
//
bool DirSlot::is_empty() const
{
    return m_free_space == m_max_free_space;
}
