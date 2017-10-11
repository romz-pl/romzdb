#include "heappage.h"
#include <cassert>

//
//
//
HeapPage::HeapPage( Page& page )
    : m_page( page )
{
    m_hdr.FromPage( page );
}

//
//
//
HeapPage::~HeapPage()
{
    m_hdr.ToPage( m_page );
}

//
//
//
Record HeapPage::Get( SlotId slotId )
{
    const Slot slot = m_hdr.GetSlot( slotId );

    const char* p = m_page.GetData();
    p += slot.m_offset;
    Record rec( p, slot.m_length );
    return rec;
}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    const auto ret = m_hdr.Insert( rec.GetLength() );
    const auto offset = ret.first;
    const auto slotId = ret.second;

    char* p = m_page.GetData() + offset;
    rec.ToPage( p );

    return slotId;
}

//
//
//
void HeapPage::Delete( SlotId slotId )
{
    m_hdr.Delete( slotId );
}
