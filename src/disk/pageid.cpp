#include "pageid.h"
#include <limits>

//
//
//
const std::uint32_t PageId::m_invalid = std::numeric_limits< std::uint32_t >::max();



//
//
//
PageId::PageId( std::uint32_t id )
    : m_id ( id )
{

}

//
//
//
std::uint32_t PageId::GetValue() const
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

