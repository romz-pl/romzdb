#include "heappage.h"
#include <cassert>

//
//
//
HeapPage::HeapPage( BufferMgr& bufferMgr, PageId self )
    : m_self( self )
    , m_hdr( bufferMgr, self )
{
}

//
//
//
HeapPage::~HeapPage()
{
}

//
//
//
Record HeapPage::Get( SlotId slotId )
{
    return m_hdr.Get( slotId );

}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    return m_hdr.Insert( rec );
}

//
//
//
PageOffset HeapPage::Delete( SlotId slotId )
{
    return m_hdr.Delete( slotId );
}

//
//
//
std::size_t HeapPage::GetRecordNo() const
{
    return  m_hdr.GetSlotNo();
}

//
//
//
std::int32_t HeapPage::GetFreeSpace() const
{
    return m_hdr.GetFreeSpace();
}
