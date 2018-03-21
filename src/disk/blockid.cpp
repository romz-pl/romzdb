#include "blockid.h"
#include <limits>


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
