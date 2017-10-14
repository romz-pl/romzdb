#include "dirpage.h"
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <algorithm>
#include "heappage.h"

//
//
//


//
//
//
DirPage::DirPage( BufferMgr& bufferMgr, PageId self )
    : m_self( self )
    , m_nextPage( InvalidPageId )
    , m_bufferMgr( bufferMgr )
{
    FromPage();
}

//
//
//
DirPage::~DirPage()
{

}

//
// Returns true, if there is no free space on the Dir Page
//
bool DirPage::IsFull() const
{
    constexpr std::size_t maxEntries =
            ( Page::PageSize - sizeof( PageId ) - sizeof( std::size_t ) ) /
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
    const auto pred = [ rec ]( const DirSlot& d ){ return ( d.m_freeSpace >= rec.GetLength() ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    if( it != m_dirSlot.end() )
    {
        const PageId pageId = it->m_pageId;
        HeapPage heapPage( m_bufferMgr, pageId );
        const SlotId slotId = heapPage.Insert( rec );
        it->m_freeSpace = heapPage.GetFreeSpace();

        ToPage();
        return std::make_pair( true, RecordId( pageId, slotId ) );
    }
    else
    {
        return std::make_pair( false, RecordId( 0, 0 ) );
    }
}

//
//
//
void DirPage::InsertPage( PageId pageId )
{
    assert( !IsFull() );
    const auto pred = [ pageId ]( const DirSlot& d ){ return ( d.m_pageId == pageId ); };
    if( std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred ) != m_dirSlot.end() )
    {
        throw std::runtime_error( "DirPage::Insert: PageId '" + std::to_string( pageId ) + "' already inserted." );
    }

    DirSlot d;
    d.m_pageId = pageId;
    d.m_freeSpace = Page::PageSize;
    m_dirSlot.push_back( d );
    ToPage();
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

        it->m_freeSpace = heapPage.GetFreeSpace();

        ToPage();
        return true;
    }

    return false;
}

//
//
//
void DirPage::ToPage() const
{
    Page* page = m_bufferMgr.GetPage( m_self, false );
    char * p = page->GetData() + Page::PageSize;

    p -= sizeof( m_nextPage );
    std::memcpy( p, &m_nextPage, sizeof( m_nextPage ) );

    const auto s = m_dirSlot.size();
    p -= sizeof( s );
    std::memcpy( p, &s, sizeof( s ) );


    p = page->GetData();
    for( DirSlot v : m_dirSlot )
    {
        std::memcpy( p, &v.m_pageId, sizeof( v.m_pageId ) );
        p += sizeof( v.m_pageId );

        std::memcpy( p, &v.m_freeSpace, sizeof( v.m_freeSpace ) );
        p += sizeof( v.m_freeSpace );
    }

    m_bufferMgr.MarkDirty( m_self );
    m_bufferMgr.UnpinPage( m_self );
}

//
//
//
void DirPage::FromPage()
{
    Page* page = m_bufferMgr.GetPage( m_self, false );
    char * p = page->GetData() + Page::PageSize;

    p -= sizeof( PageId );
    std::memcpy( &m_nextPage, p, sizeof( PageId ) );

    p -= sizeof( std::size_t );
    std::size_t s = 0;
    std::memcpy( &s, p, sizeof( std::size_t ) );

    p = page->GetData();
    DirSlot v;
    for( std::size_t i = 0; i < s; i++ )
    {
        std::memcpy( &v.m_pageId, p, sizeof( v.m_pageId ) );
        p += sizeof( v.m_pageId );

        std::memcpy( &v.m_freeSpace, p, sizeof( v.m_freeSpace ) );
        p += sizeof( v.m_freeSpace );

        m_dirSlot.push_back( v );
    }

    m_bufferMgr.UnpinPage( m_self );
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
bool DirPage::Is( PageId pageId ) const
{
    auto pred = [ pageId ]( DirSlot d ){ return ( d.m_pageId == pageId ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    return ( it != m_dirSlot.end() );
}

//
//
//
PageId DirPage::GetPageId() const
{
    return m_self;
}

