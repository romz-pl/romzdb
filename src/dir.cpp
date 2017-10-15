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
    PageId currPage = headerPage;

    while( currPage.IsValid() )
    {
        DirPage dp( m_bufferMgr, currPage );
        m_dirPage.push_back( dp );
        currPage = dp.GetNextPage();
    }
}

//
//
//
Dir::~Dir()
{

}

//
//
//
Record Dir::Get( RecordId rid ) const
{
    for( const DirPage& d : m_dirPage )
    {
        auto pair = d.Get( rid );
        if( pair.first )
            return pair.second;
    }

    throw std::runtime_error( "Dir::Get: Record not found." );

}

//
//
//
RecordId Dir::Insert( const Record& rec )
{
    for( DirPage& d : m_dirPage )
    {
        auto pair = d.Insert( rec );
        if( pair.first )
            return pair.second;
    }


    InsertHeapPage();

    return Insert( rec );
}

//
//
//
PageId Dir::InsertHeapPage()
{
    auto pair = m_bufferMgr.GetNew( );
    PageId pageId = pair.first;

    for( DirPage& d : m_dirPage )
    {
        if( !d.IsFull( ) )
        {
            d.InsertPage( pageId );
            m_bufferMgr.Unpin( pageId );
            return pageId;
        }
    }

    //
    // The directry is full. New page for directory must be allocated.
    //
    m_dirPage.back().SetNextPage( pageId );
    m_dirPage.push_back( DirPage( m_bufferMgr, pageId ) );
    m_bufferMgr.Unpin( pageId );

    pair = m_bufferMgr.GetNew( );
    pageId = pair.first;
    m_dirPage.back().InsertPage( pageId );
    m_bufferMgr.Unpin( pageId );

    return pageId;
}

//
//
//
void Dir::Delete( RecordId rid )
{
    for( DirPage& d : m_dirPage )
    {
        if( d.Delete( rid ) )
            return;
    }

    throw std::runtime_error( "Dir::Delete: RecordId not found." );
}
