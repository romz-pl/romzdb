#include <limits>
#include <stdexcept>
#include "dirslot.h"
#include "disk/diskblock.h"
#include "heappage.h"


const std::uint32_t DirSlot::m_max_free_space = HeapPage::GetMaxRecordLength();


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
        m_free_space = hp.GetFreeSpace();
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

    const PageId page_id = record_id.get_page_id();

    if( m_page_id != page_id )
    {
        return false;
    }

    { // Must be in local scope!
        HeapPage hp( buffer, page_id );
        hp.Remove( record_id.get_slot_id() );
        m_free_space = hp.GetFreeSpace();
    }

    if( is_empty() )
    {
        buffer.dispose( page_id );
        m_valid = false;
    }
    return true;
}

//
//
//
bool DirSlot::alloc_page( BufferMgr& buffer )
{
    if( !m_valid )
    {
        m_page_id = buffer.alloc().first;
        buffer.unpin( m_page_id, true );

        m_free_space = m_max_free_space;
        m_valid = true;
        return true;
    }

    return false;
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
