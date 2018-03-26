#include "recordid.h"

//
//
//
RecordId::RecordId( PageId page_id, SlotId slot_id )
    : m_page_id( page_id )
    , m_slot_id( slot_id )
{

}

//
//
//
PageId RecordId::get_page_id() const
{
    return m_page_id;
}

//
//
//
SlotId RecordId::get_slot_id() const
{
    return m_slot_id;
}

//
//
//
bool RecordId::operator== ( const RecordId& rid ) const
{
    return ( m_page_id == rid.m_page_id) && ( m_slot_id == rid.m_slot_id );
}

//
//
//
bool RecordId::operator< ( const RecordId& rid ) const
{
    if( m_page_id == rid.m_page_id )
    {
        return m_slot_id < rid.m_slot_id;
    }

    return ( m_page_id < rid.m_page_id );
}
