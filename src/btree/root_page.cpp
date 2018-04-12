#include "root_page.h"

/*
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

    for( auto it = begin(); it != end(); ++it )
    {
        it->make_invalid( );
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

    for( auto it = begin(); it != end(); ++it )
    {
        auto ret = it->insert_record( m_buffer, rec );
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
    for( auto it = begin(); it != end(); ++it )
    {
        if( it->remove_record( m_buffer, record_id ) )
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
    for( auto it = begin(); it != end(); ++it )
    {
        if( it->alloc_page( m_buffer ) )
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
    for( auto it = begin(); it != end(); ++it )
    {
        ret += it->get_record_no( m_buffer );
    }
    return ret;
}

//
//
//
void DirPage::get_all_records( std::vector< Record >& all ) const
{
    for( auto it = begin(); it != end(); ++it )
    {
        it->get_all_records( m_buffer, all );
    }
}

//
//
//
void DirPage::get_all_rids( std::vector< RecordId >& all ) const
{
    for( auto it = begin(); it != end(); ++it )
    {
        it->get_all_rids( m_buffer, all );
    }
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

//
//
//
DirPage::iterator DirPage::begin()
{
    return iterator( get_slot() );
}


//
//
//
DirPage::const_iterator DirPage::begin() const
{
    return const_iterator( get_slot() );
}

//
//
//
DirPage::iterator DirPage::end()
{
    return iterator( get_slot() + max_slot_no() );
}

//
//
//
DirPage::const_iterator DirPage::end() const
{
    return const_iterator( get_slot() + max_slot_no() );
}

*/

