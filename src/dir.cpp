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
void Dir::InsertHeapPage()
{
    PageId pageId = m_bufferMgr.GetNew( );

    for( DirPage& d : m_dirPage )
    {
        if( !d.IsFull( ) )
        {
            d.InsertPage( pageId );
            return;
        }
    }

    //
    // The directry is full. New page for directory must be allocated.
    //
    m_dirPage.back().SetNextPage( pageId );
    m_dirPage.push_back( DirPage( m_bufferMgr, pageId ) );

    pageId = m_bufferMgr.GetNew( );
    m_dirPage.back().InsertPage( pageId );
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
