#include "dir.h"
#include <string>
#include <stdexcept>
#include <cassert>

//
//
//
Dir::Dir( BufferMgr& bufferMgr, PageId headerPage )
    : m_bufferMgr( bufferMgr )
{
    PageId nextPage = headerPage;

    while( nextPage != InvalidPageId )
    {
        Page* page = m_bufferMgr.GetPage( nextPage, false );
        DirPage dp( *page, nextPage );
        m_dirPage.push_back( dp );
        nextPage = dp.GetNextPage();
    }
}

//
//
//
Dir::~Dir()
{
    for( const DirPage& d : m_dirPage )
    {
        m_bufferMgr.UnpinPage( d.GetPageId() );
    }
}

//
//
//
bool Dir::Is( PageId pageId ) const
{
    for( const DirPage& d : m_dirPage )
    {
        if( d.Is( pageId ) )
            return true;
    }
    return false;
}

//
//
//
PageId Dir::Insert()
{
    auto pair = m_bufferMgr.GetNewPage( );
    PageId pageId = pair.first;
    Page* page = pair.second;

    for( DirPage& d : m_dirPage )
    {
        if( !d.IsFull( ) )
        {
            d.Insert( pageId );
            m_bufferMgr.UnpinPage( pageId );
            return pageId;
        }
    }

    //
    // The directry is full. New page for directory must be allocated.
    //
    m_dirPage.back().SetNextPage( pageId );
    m_dirPage.push_back( DirPage( *page, pageId ) );

    pair = m_bufferMgr.GetNewPage( );
    pageId = pair.first;
    page = pair.second;
    m_dirPage.back().Insert( pageId );
    m_bufferMgr.UnpinPage( pageId );

    return pageId;
}

//
//
//
void Dir::Delete( PageId pageId )
{
    for( DirPage& d : m_dirPage )
    {
        if( d.Delete( pageId ) )
            return;
    }

    throw std::runtime_error( "Dir::Delete: Page '" + std::to_string( pageId ) + "' not found." );
}
