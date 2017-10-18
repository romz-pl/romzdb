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
    return m_dir.Get( rid );
}

//
//
//
RecordId HeapFile::Insert( const Record& rec )
{
    const std::size_t maxRecordLength = HeapPage::GetMaxRecordLength();

    const PageOffset recLength = rec.GetLength();
    if( PageOffset( maxRecordLength ) < recLength )
    {
        throw std::runtime_error( "Record is too long. It does not fix onto one page."
                                  "Current version of the romzdb does not support this." );
    }

    return m_dir.Insert( rec );
}

//
//
//
void HeapFile::Delete( RecordId rid )
{
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

//
//
//
std::size_t HeapFile::GetRecordNo() const
{
    return m_dir.GetRecordNo( );
}

//
//
//
void HeapFile::GetRid( std::vector< RecordId >& rid ) const
{
    m_dir.GetRid( rid );
}
