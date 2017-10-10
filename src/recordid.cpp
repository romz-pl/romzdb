#include "recordid.h"

//
//
//
RecordId::RecordId( PageId pageId, SlotId slotId )
    : m_pageId( pageId )
    , m_slotId( slotId )
{

}

//
//
//
PageId RecordId::GetPageId() const
{
    return m_pageId;
}

//
//
//
SlotId RecordId::GetSlotId() const
{
    return m_slotId;
}
