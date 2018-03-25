#include "dirslot.h"

//
//
//
DirSlot::DirSlot( PageId pageId, PageOffset freeSpace )
    : m_pageId( pageId )
    , m_freeSpace( freeSpace )
{

}

//
// Returns "true" if there is enought free space on "m_pageId"
// for inserting record "rec"
//
bool DirSlot::IsFree( const Record &rec ) const
{
    std::int32_t free = m_freeSpace.GetValue();
    free -= rec.GetLength().GetValue();
    free -= 2 * sizeof( PageOffset ); // sizeof(Slot)
    return ( free >= 0 );
}
