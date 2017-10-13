#include "dirpage.h"
#include <cassert>
#include <algorithm>
#include <stdexcept>

//
//
//
const std::size_t DirPage::m_maxEntries =
        ( Page::PageSize - sizeof( PageId ) - sizeof( std::size_t ) ) / sizeof( PageId );

//
//
//
DirPage::DirPage( Page& page )
    : m_page( page )
{
    FromPage();
}

//
// Returns true, if there is no free space on the Dir Page
//
bool DirPage::IsFull() const
{
    return ( m_pageId.size() < m_maxEntries );
}

//
//
//
void DirPage::Insert( PageId pageId )
{
    assert( !IsFull() );
    if( std::find( m_pageId.begin(), m_pageId.end(), pageId ) != m_pageId.end() )
    {
        throw std::runtime_error( "DirPage::Insert: PageId '" + std::to_string( pageId ) + "' already inserted." );
    }

    m_pageId.push_back( pageId );
    ToPage();
}

//
//
//
void DirPage::Delete( PageId pageId )
{
    auto it = std::find( m_pageId.begin(), m_pageId.end(), pageId );
    if( it == m_pageId.end() )
    {
        throw std::runtime_error( "DirPage::Delete: PageId '" + std::to_string( pageId ) + "' not found." );
    }

    m_pageId.erase( it );
    ToPage();
}

//
//
//
void DirPage::ToPage() const
{
    char * p = m_page.GetData() + Page::PageSize;

    p -= sizeof( PageId );
    std::memcpy( p, &m_nextPage, sizeof( PageId ) );

    p -= sizeof( std::size_t );
    const std::size_t s = m_pageId.size();
    std::memcpy( p, &s, sizeof( std::size_t ) );


    p = m_page.GetData();
    for( PageId v : m_pageId )
    {
        std::memcpy( p, &v, sizeof( PageId ) );
        p += sizeof( PageId );
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
    PageId v = 0;
    for( std::size_t i = 0; i < s; i++ )
    {
        std::memcpy( &v, p, sizeof( PageId ) );
        m_pageId.push_back( v );
        p += sizeof( PageId );
    }
}


