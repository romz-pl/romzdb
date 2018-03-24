#include "dbfileid.h"


//
//
//
DbFileId::DbFileId( std::uint32_t id )
    : m_id ( id )
{

}

//
//
//
bool DbFileId::operator== ( const DbFileId& v ) const
{
    return ( m_id == v.m_id );
}

//
//
//
bool DbFileId::operator< ( const DbFileId& v ) const
{
    return ( m_id < v.m_id );
}
