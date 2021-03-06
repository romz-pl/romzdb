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

//
//
//
bool RecordId::operator == ( const RecordId& rid ) const
{
    return ( m_pageId == rid.m_pageId) && ( m_slotId == rid.m_slotId );
}
