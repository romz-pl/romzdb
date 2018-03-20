#include "pageid.h"
#include <limits>

//
//
//
const std::size_t PageId::m_invalid = std::numeric_limits< std::size_t >::max();



//
//
//
PageId::PageId( std::size_t id )
    : m_id ( id )
{

}

//
//
//
std::size_t PageId::GetValue() const
{
    return m_id;
}

//
//
//
bool PageId::IsValid() const
{
    return ( m_id != m_invalid );
}

//
//
//
bool PageId::operator== ( const PageId& v ) const
{
    return ( v.m_id == m_id );
}

