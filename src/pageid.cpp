#include "pageid.h"
#include <limits>

//
//
//
const std::size_t PageId::m_invalid = std::numeric_limits< std::size_t >::max();

//
//
//
PageId::PageId()
    : m_id( m_invalid )
{

}

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
