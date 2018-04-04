#include "dirpage.h"
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <cstring>
#include <utility>
#include "dirslot.h"


//
//
//
DirPage::DirPage( DiskBlock* block )
    : m_block( block )
{

}

//
//
//
DirPage::~DirPage()
{

}

//
//
//
void DirPage::init( )
{
    set_next_page( PageId( 0, 0 ) );

    DirSlot *slot = (DirSlot*)( m_block->GetData() + Offset::Array );

    for( std::uint32_t i = 0; i < max_slot_no(); i++, slot++ )
    {
        slot->make_invalid( );
    }
}

//
//
//
std::optional< PageId > DirPage::insert_record( std::uint32_t count )
{
    DirSlot *slot = (DirSlot*)( m_block->GetData() + Offset::Array );

    for( std::uint32_t i = 0; i < max_slot_no(); i++, slot++ )
    {
        if( slot->insert_record( count ) )
        {
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
    DirSlot *slot = (DirSlot*)( m_block->GetData() + Offset::Array );

    for( std::uint32_t i = 0; i < max_slot_no(); i++, slot++ )
    {
        if( slot->remove_record( page_id, count ) )
        {
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
    DirSlot *slot = (DirSlot*)( m_block->GetData() + Offset::Array );

    for( std::uint32_t i = 0; i < max_slot_no(); i++, slot++ )
    {
        if( slot->alloc_page( page_id ) )
        {
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
    DirSlot *slot = (DirSlot*)( m_block->GetData() + Offset::Array );

    for( std::uint32_t i = 0; i < max_slot_no(); i++, slot++ )
    {
        if( slot->dispose_page( page_id ) )
        {
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
    constexpr std::uint32_t v =
        ( DiskBlock::Size - sizeof( PageId ) - sizeof( std::uint32_t ) )
         / ( sizeof( DirSlot ) );

    return v;
}


//
//
//
bool DirPage::is_next_page() const
{
    PageId *p = (PageId*)( m_block->GetData() + Offset::Next_page );
    const PageId id = *p;
    return ( id != PageId( 0, 0 ) );
}


//
//
//
PageId DirPage::get_next_page() const
{
    PageId *p = (PageId*)( m_block->GetData() + Offset::Next_page );
    const PageId id = *p;
    return id;
}

//
//
//
void DirPage::set_next_page( PageId id )
{
    PageId *p = (PageId*)( m_block->GetData() + Offset::Next_page );
    *p = id;
}




/*
//
// Returns true, if there is no free space on the Dir Page
//
bool DirPage::IsFull() const
{
    constexpr std::size_t maxEntries =
            ( Page::Size - sizeof( PageId ) - sizeof( std::size_t ) ) /
            ( sizeof( PageId ) + sizeof( std::size_t ) );

    return ( m_dirSlot.size() >= maxEntries );
}

//
//
//
std::pair< bool, Record > DirPage::Get( RecordId rid ) const
{
    const auto pred = [ rid ]( const DirSlot& d ){ return ( d.m_pageId == rid.GetPageId() ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    if( it != m_dirSlot.end() )
    {
        const PageId pageId = rid.GetPageId();
        const SlotId slotId = rid.GetSlotId();
        HeapPage heapPage( m_bufferMgr, pageId );

        const Record rec = heapPage.Get( slotId );
        return std::make_pair( true, rec );
    }
    else
    {
        return std::make_pair( false, Record("") );
    }
}

//
//
//
std::pair< bool, RecordId > DirPage::Insert( const Record &rec )
{
    using namespace std::rel_ops;

    const auto pred = [ rec ]( const DirSlot& d ){ return d.IsFree( rec ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    if( it != m_dirSlot.end() )
    {
        const PageId pageId = it->m_pageId;
        HeapPage heapPage( m_bufferMgr, pageId );
        const SlotId slotId = heapPage.Insert( rec );
        it->m_freeSpace = PageOffset( heapPage.GetFreeSpace() );

        ToPage();
        return std::make_pair( true, RecordId( pageId, slotId ) );
    }
    else
    {
        return std::make_pair( false, RecordId( PageId( 0, 0 ), SlotId( 0 ) ) );
    }
}

//
//
//
bool DirPage::InsertHeapPage( PageId pageId )
{
    if( IsFull() )
        return false;

    const auto pred = [ pageId ]( const DirSlot& d ){ return ( d.m_pageId == pageId ); };
    if( std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred ) != m_dirSlot.end() )
    {
        throw std::runtime_error( "DirPage::Insert: PageId already inserted." );
    }

    DirSlot d( pageId, PageOffset( Page::Size ) );
    m_dirSlot.push_back( d );
    ToPage();
    return true;
}


//
//
//
bool DirPage::Delete( RecordId rid )
{
    const auto pred = [ rid ]( const DirSlot& d ){ return ( d.m_pageId == rid.GetPageId() ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    if( it != m_dirSlot.end() )
    {
        const PageId pageId = rid.GetPageId();
        HeapPage heapPage( m_bufferMgr, pageId );
        const SlotId slotId = rid.GetSlotId();
        heapPage.Delete( slotId );

        it->m_freeSpace = PageOffset( heapPage.GetFreeSpace() );

        ToPage();
        return true;
    }

    return false;
}

//
//
//
void DirPage::ToPage()
{
    char * p = GetData() + Page::Size;

    p -= sizeof( m_nextPage );
    std::memcpy( p, &m_nextPage, sizeof( m_nextPage ) );

    const auto s = m_dirSlot.size();
    p -= sizeof( s );
    std::memcpy( p, &s, sizeof( s ) );


    p = GetData();
    for( DirSlot v : m_dirSlot )
    {
        std::memcpy( p, &v.m_pageId, sizeof( v.m_pageId ) );
        p += sizeof( v.m_pageId );

        std::memcpy( p, &v.m_freeSpace, sizeof( v.m_freeSpace ) );
        p += sizeof( v.m_freeSpace );
    }
}

//
//
//
void DirPage::FromPage()
{
    char * p = GetData() + Page::Size;

    p -= sizeof( PageId );
    std::memcpy( &m_nextPage, p, sizeof( PageId ) );

    p -= sizeof( std::size_t );
    std::size_t s = 0;
    std::memcpy( &s, p, sizeof( std::size_t ) );

    p = GetData();
    for( std::size_t i = 0; i < s; i++ )
    {
        PageId pageId( 0, 0 );
        PageOffset freeSpace( 0 );
        std::memcpy( &pageId, p, sizeof( pageId ) );
        p += sizeof( pageId );

        std::memcpy( &freeSpace, p, sizeof( freeSpace ) );
        p += sizeof( freeSpace );

        m_dirSlot.push_back( DirSlot( pageId, freeSpace ) );
    }
}

//
//
//
PageId DirPage::GetNextPage() const
{
    return m_nextPage;
}

//
//
//
void DirPage::SetNextPage( PageId id )
{
    m_nextPage = id;
    ToPage();
}

//
//
//
std::size_t DirPage::GetRecordNo() const
{
    std::size_t ret = 0;
    for( DirSlot v : m_dirSlot )
    {
        HeapPage hp( m_bufferMgr, v.m_pageId );
        ret += hp.GetRecordNo();
    }
    return ret;
}

//
//
//
void DirPage::GetRid( std::vector< RecordId >& rid ) const
{
    for( DirSlot v : m_dirSlot )
    {
        HeapPage hp( m_bufferMgr, v.m_pageId );
        hp.GetRid( rid );
    }
}
*/
