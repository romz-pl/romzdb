#include "dir.h"
#include <string>
#include <stdexcept>

//
//
//
Dir::Dir( BufferMgr& bufferMgr, PageId /*headerPage*/ )
    : m_bufferMgr( bufferMgr )
{
    /*
    Page* page = m_bufferMgr.GetPage( headerPage, false );
    m_dirPage.push_back( DirPage( page ) );
    */
}

//
//
//
bool Dir::Is( PageId /*pagId*/ ) const
{
    /*
    for( DirPage& d : m_dirPage )
    {
        if( d.Is( pageId ) )
            return true;
    }
    return false;
    */

    return false;
}

//
//
//
PageId Dir::Insert()
{
    /*
    const auto pair = m_bufferMgr.GetNewPage( );
    const PageId pageId = pair.first;
    Page* page = pair.second;

    for( DirPage& d : m_dirPage )
    {
        if( !d.IsFull( ) )
        {
            d.Insert( pageId );
            return pageId;
        }
    }


    m_dirPage.back().SetNext( pageId );
    m_dirPage.push_back( DirPage( page ) );
    */
    return 0;

}

//
//
//
void Dir::Delete( PageId /*pageId*/ )
{
    /*
    for( DirPage& d : m_dirPage )
    {
        if( d.Delete( pageId ) )
            return;
    }

    throw std::runtime_error( "Dir::Delete: Page '" + std::to_string( pageId ) + "' not found." );
    */
}
