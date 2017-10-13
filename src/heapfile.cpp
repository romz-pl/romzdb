#include "heapfile.h"
#include <cassert>
#include "heappage.h"

//
//
//
HeapFile::HeapFile( BufferMgr& bufferMgr, PageId headerPage )
    : m_bufferMgr( bufferMgr )
    , m_headerPage( headerPage )
{

}


//
//
//
Record HeapFile::Get( RecordId rid )
{
    const PageId pageId = rid.GetPageId();
    // assert( m_dir.Is( pageId ) );
    Page* page = m_bufferMgr.GetPage( pageId, true );
    assert( page );
    HeapPage hp( *page );
    const Record rec = hp.Get( rid.GetSlotId() );
    m_bufferMgr.UnpinPage( pageId );

    return rec;
}

//
//
//
RecordId HeapFile::Insert( const Record& /*rec*/ )
{
//    HeapDir heapDir;
//    const PageId pageId = heapDir.Insert();
//    HeapPage heapPage( m_bufferMgr.GetPage( pageId, true ) );
//    const SlotId slotId = heapPage.Insert( rec );
//    m_bufferMgr.UnpinPage( pageId );
//
//    return RecordId( pageId, slotId );

    return RecordId( 0, 0 );
}

//
//
//
void HeapFile::Delete( RecordId /*rid*/ )
{
//    HeapPage heapPage( m_bufferMgr.GetPage( rid.GetPageId(), true ) );
//    heapPage.Delete( rid.GetSlotId() );
//    m_bufferMgr.UnpinPage( rid.GetPageId() );

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
