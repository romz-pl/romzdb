#include "dirpage.h"


//
//
//
DirPage::DirPage( BufferMgr& buffer )
    : m_buffer( buffer )
    , m_page_id( 0, 0 )
    , m_block( nullptr )
    , m_dirty( false )
{
    auto ret = m_buffer.alloc();
    m_page_id = ret.first;
    m_block = ret.second;

    init();
}


//
//
//
DirPage::DirPage( BufferMgr& buffer, PageId page_id )
    : m_buffer( buffer )
    , m_page_id( page_id )
    , m_block( nullptr )
    , m_dirty( false )
{
    m_block = m_buffer.pin( page_id );
}

//
//
//
DirPage::~DirPage()
{
    m_buffer.unpin( m_page_id, m_dirty );
}

//
//
//
PageId DirPage::get_page_id( ) const
{
    return m_page_id;
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
    set_next_page( PageId( 0, 0 ) );

    DirSlot *slot = get_slot();
    DirSlot * const slot_end = slot + max_slot_no();

    while( slot != slot_end )
    {
        slot->make_invalid( );
        slot++;
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
    return ( id != PageId( 0, 0 ) );
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
