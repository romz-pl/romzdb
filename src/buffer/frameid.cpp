#include "frameid.h"


//
//
//
FrameId::FrameId( std::uint32_t id )
    : m_id ( id )
{

}

void FrameId::inc( std::uint32_t frame_no )
{
    m_id = ( m_id + 1 ) % frame_no;
}

//
//
//
std::uint32_t FrameId::to_uint32() const
{
    return m_id;
}

/*
//
//
//
std::uint32_t FrameId::GetValue() const
{
    return m_id;
}

//
//
//
bool FrameId::operator== ( const FrameId& v ) const
{
    return ( m_id == v.m_id );
}

//
//
//
bool FrameId::operator< ( const FrameId& v ) const
{
    return ( m_id < v.m_id );
}
*/
