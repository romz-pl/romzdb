#include "heappage.h"
#include <cassert>

//
//
//
HeapPage::HeapPage( Page* page )
{

}

//
//
//
Record HeapPage::Get( SlotId slotId )
{
    // assert( slotId < GetSlotNo() );
    // page->Get
}

//
//
//
SlotId HeapPage::Insert( const Record& rec )
{

}

//
//
//
void HeapPage::Delete( SlotId slotId )
{

}
