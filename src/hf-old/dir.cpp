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
    PageId pageId = m_bufferMgr.alloc( ).first;
    m_bufferMgr.unpin( pageId, false );

    for( DirPage& d : m_dirPage )
    {
        if( d.InsertHeapPage( pageId ) )
        {
            return;
        }
    }

    //
    // The directry is full. New page for directory must be allocated.
    //
    m_dirPage.back().SetNextPage( pageId );
    DirPage dp( m_bufferMgr, pageId );

    pageId = m_bufferMgr.alloc( ).first;
    m_bufferMgr.unpin( pageId, false );
    dp.InsertHeapPage( pageId );

    m_dirPage.push_back( dp );
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

//
//
//
std::size_t Dir::GetRecordNo() const
{
    std::size_t ret = 0;

    for( const DirPage& d : m_dirPage )
    {
        ret += d.GetRecordNo();
    }
    return ret;
}

//
//
//
void Dir::GetRid( std::vector< RecordId >& rid ) const
{
    for( const DirPage& d : m_dirPage )
    {
        d.GetRid( rid );
    }
}
