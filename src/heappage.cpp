#include "heappage.h"
#include <cassert>

//
//
//
HeapPage::HeapPage( Page* page )
    : m_page( page )
{
    m_hdr.FromPage( *page );
}

//
//
//
Record HeapPage::Get( SlotId slotId )
{
    assert( slotId < m_hdr.GetSlotNo() );
    const Slot slot = m_hdr.GetSlot( slotId );

    const char* p = m_page->GetData();
    p += slot.m_offset;
    Record rec( p, slot.m_length );
    return rec;
}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{
    const PageOffset slotId = m_hdr.Insert( rec.GetSize() );
    const Slot slot = m_hdr.GetSlot( slotId );

    char* p = m_page->GetData();
    p += slot.m_offset;
    rec.ToPage( p );

    m_hdr.ToPage( *m_page );


    return slotId;
}

//
//
//
void HeapPage::Delete( SlotId slotId )
{
    m_hdr.Delete( slotId );
    m_hdr.ToPage( *m_page );
}
