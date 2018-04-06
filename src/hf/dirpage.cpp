#include "dirpage.h"


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
    DirSlot *slot = reinterpret_cast< DirSlot* >( m_block->GetData() + Offset::Slot );
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
std::optional< PageId > DirPage::insert_record( std::uint32_t count )
{
    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        if( slot->insert_record( count ) )
        {
            m_dirty = true;
            return slot->get_page_id();
        }
    }
    return std::nullopt;
}

//
//
//
bool DirPage::remove_record( PageId page_id, std::uint32_t count )
{
    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        if( slot->remove_record( page_id, count ) )
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
bool DirPage::alloc_page( PageId page_id )
{
    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    for( ; slot != slot_end; slot++ )
    {
        if( slot->alloc_page( page_id ) )
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
std::uint32_t DirPage::max_slot_no() const
{
    constexpr std::uint32_t v = ( DiskBlock::Size - sizeof( PageId ) ) / ( sizeof( DirSlot ) );

    return v;
}


//
//
//
bool DirPage::is_next_page() const
{
    PageId *p = reinterpret_cast< PageId* >( m_block->GetData() + Offset::Next_page );
    const PageId id = *p;
    return ( id != m_invalid_page_id );
}


//
//
//
PageId DirPage::get_next_page() const
{
    PageId *p = reinterpret_cast< PageId* >( m_block->GetData() + Offset::Next_page );
    const PageId id = *p;
    return id;
}

//
//
//
void DirPage::set_next_page( PageId id )
{
    PageId *p = reinterpret_cast< PageId* >( m_block->GetData() + Offset::Next_page );
    *p = id;
    m_dirty = true;
}
