#include "dirpage.h"
#include "heappage.h"


const PageId DirPage::m_invalid_page_id = PageId( 0 , 0 );

//
//
//
DirPage::DirPage( BufferMgr& buffer )
    : Page( buffer )
{

    init();
}


//
//
//
DirPage::DirPage( BufferMgr& buffer, PageId page_id )
    : Page( buffer, page_id )
{

}

//
//
//
DirSlot* DirPage::get_slot()
{
    DirSlot *slot = reinterpret_cast< DirSlot* >( get_data() + Offset::Slot );
    return slot;
}

//
//
//
const DirSlot* DirPage::get_slot() const
{
    const DirSlot *slot = reinterpret_cast< const DirSlot* >( get_data() + Offset::Slot );
    return slot;
}

//
//
//
void DirPage::init( )
{
    set_next_page( m_invalid_page_id );

    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        slot->make_invalid( );
    }
    m_dirty = true;
}

//
//
//
std::optional< RecordId > DirPage::insert_record( const Record& rec )
{
    if( rec.get_length() > HeapPage::GetMaxRecordLength() )
    {
        throw std::runtime_error( "DirPage::insert_record: record too long" );
    }

    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        auto ret = slot->insert_record( m_buffer, rec );
        if( ret.has_value() )
        {
            m_dirty = true;
            return ret.value();
        }
    }
    return std::nullopt;
}

//
//
//
bool DirPage::remove_record( RecordId record_id )
{
    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        if( slot->remove_record( m_buffer, record_id ) )
        {
            m_dirty = true;
            return true;
        }
    }
    return false;
}

//
//
//
bool DirPage::alloc_page( )
{
    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        if( slot->alloc_page( m_buffer ) )
        {
            m_dirty = true;
            return true;
        }
    }
    return false;
}

//
//
//
bool DirPage::dispose_page( PageId page_id )
{
    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        if( slot->dispose_page( page_id ) )
        {
            m_dirty = true;
            return true;
        }
    }
    return false;
}

//
//
//
std::uint32_t DirPage::get_record_no() const
{
    std::uint32_t ret = 0;
    const DirSlot *slot = get_slot();
    const DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        ret += slot->get_record_no( m_buffer );
    }
    return ret;
}


//
//
//
std::uint32_t DirPage::max_slot_no()
{
    constexpr std::uint32_t v = ( DiskBlock::Size - sizeof( PageId ) ) / ( sizeof( DirSlot ) );

    return v;
}


//
//
//
bool DirPage::is_next_page() const
{
    const PageId *p = reinterpret_cast< const PageId* >( get_data() + Offset::Next_page );
    const PageId id = *p;
    return ( id != m_invalid_page_id );
}


//
//
//
PageId DirPage::get_next_page() const
{
    const PageId *p = reinterpret_cast< const PageId* >( get_data() + Offset::Next_page );
    const PageId id = *p;
    return id;
}

//
//
//
void DirPage::set_next_page( PageId id )
{
    PageId *p = reinterpret_cast< PageId* >( get_data() + Offset::Next_page );
    *p = id;
    m_dirty = true;
}
