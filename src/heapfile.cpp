#include "heapfile.h"
#include <cassert>
#include "heappage.h"
#include <stdexcept>

//
//
//
HeapFile::HeapFile( BufferMgr& bufferMgr, PageId headerPage )
    : m_bufferMgr( bufferMgr )
    , m_dir( bufferMgr, headerPage )
{

}


//
//
//
Record HeapFile::Get( RecordId rid )
{
    /*
    const PageId pageId = rid.GetPageId();
    if( !m_dir.Is( pageId ) )
    {
        throw std::runtime_error( "HeapFile::Get: Record not in heap file." );
    }

    Page* page = m_bufferMgr.GetPage( pageId, true );
    assert( page );
    HeapPage hp( *page );
    const Record rec = hp.Get( rid.GetSlotId() );
    m_bufferMgr.UnpinPage( pageId );

    return rec;
    */
    return m_dir.Get( rid );
}

//
//
//
RecordId HeapFile::Insert( const Record& rec )
{
    /*
    const PageId pageId = m_dir.Insert( rec.GetLength() );
    Page* page = m_bufferMgr.GetPage( pageId, true );
    HeapPage heapPage( *page );
    const SlotId slotId = heapPage.Insert( rec );
    m_bufferMgr.UnpinPage( pageId );

    return RecordId( pageId, slotId );
    */
    return m_dir.Insert( rec );
}

//
//
//
void HeapFile::Delete( RecordId rid )
{
    /*
    const PageId pageId = rid.GetPageId();
    Page* page = m_bufferMgr.GetPage( pageId, true );
    HeapPage heapPage( *page );

    const SlotId slotId = rid.GetSlotId();
    const std::size_t freeSpace = heapPage.Delete( slotId );
    m_bufferMgr.UnpinPage( rid.GetPageId() );

    m_dir.Delete( pageId, freeSpace );
    */
    m_dir.Delete( rid );
}

//
//
//
void HeapFile::Update( RecordId rid, const Record& rec )
{
    //
    // This can be done more efficiently for records of the same length.
    // But deleting "old" record, and inserting "new" record is straithforward.
    //

    Delete( rid );
    Insert( rec );
}
