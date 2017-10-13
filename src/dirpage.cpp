#include "dirpage.h"
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <algorithm>

//
//
//
const std::size_t DirPage::m_maxEntries =
        ( Page::PageSize - sizeof( PageId ) - sizeof( std::size_t ) ) /
        ( sizeof( PageId ) + sizeof( std::size_t ) );

//
//
//
DirPage::DirPage(Page& page, PageId self )
    : m_self( self )
    , m_nextPage( InvalidPageId )
    , m_page( page )
{
    FromPage();
}

//
// Returns true, if there is no free space on the Dir Page
//
bool DirPage::IsFull() const
{
    return ( m_dirSlot.size() >= m_maxEntries );
}

//
//
//
std::pair< bool, PageId > DirPage::InsertRec( std::size_t recLength )
{
    auto pred = [ recLength ]( DirSlot& d ){ return ( d.m_freeSpace >= recLength ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    if( it != m_dirSlot.end() )
    {
        it->m_freeSpace -= recLength;
        ToPage();
        return std::make_pair( true, it->m_pageId );
    }
    else
    {
        return std::make_pair( false, 0 );
    }
}

//
//
//
void DirPage::InsertPage( PageId pageId )
{
    assert( !IsFull() );
    auto pred = [ pageId ]( DirSlot& d ){ return ( d.m_pageId == pageId ); };
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
bool DirPage::Delete( PageId pageId, PageOffset freeSpace )
{
    auto pred = [ pageId ]( DirSlot& d ){ return ( d.m_pageId == pageId ); };
    auto it = std::find_if( m_dirSlot.begin(), m_dirSlot.end(), pred );
    if( it == m_dirSlot.end() )
    {
        return false;
    }

    it->m_freeSpace = freeSpace;
    ToPage();
    return true;
}

//
//
//
void DirPage::ToPage() const
{
    char * p = m_page.GetData() + Page::PageSize;

    p -= sizeof( m_nextPage );
    std::memcpy( p, &m_nextPage, sizeof( m_nextPage ) );

    const auto s = m_dirSlot.size();
    p -= sizeof( s );
    std::memcpy( p, &s, sizeof( s ) );


    p = m_page.GetData();
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
    char * p = m_page.GetData() + Page::PageSize;

    p -= sizeof( PageId );
    std::memcpy( &m_nextPage, p, sizeof( PageId ) );

    p -= sizeof( std::size_t );
    std::size_t s = 0;
    std::memcpy( &s, p, sizeof( std::size_t ) );

    p = m_page.GetData();
    DirSlot v;
    for( std::size_t i = 0; i < s; i++ )
    {
        std::memcpy( &v.m_pageId, p, sizeof( v.m_pageId ) );
        p += sizeof( v.m_pageId );

        std::memcpy( &v.m_freeSpace, p, sizeof( v.m_freeSpace ) );
        p += sizeof( v.m_freeSpace );

        m_dirSlot.push_back( v );
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

