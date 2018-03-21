#include "blockid.h"
#include <limits>

//
//
//
const std::uint32_t BlockId::m_invalid = std::numeric_limits< std::uint32_t >::max();

//
//
//
BlockId::BlockId( )
    : m_id ( m_invalid )
{

}

//
//
//
BlockId::BlockId( std::uint32_t id )
    : m_id ( id )
{

}

//
//
//
std::uint32_t BlockId::GetValue() const
{
    return m_id;
}
