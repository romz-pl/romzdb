#include <limits>
#include "dirslot.h"

//
//
//
DirSlot::DirSlot( PageId page_id, std::uint32_t free_space )
    : m_page_id( page_id )
    , m_free_space( free_space )
{

}


//
// Returns "true" if there is enought free space on "m_pageId"
//
bool DirSlot::is_free( std::uint32_t free_space ) const
{
    return ( m_free_space >= free_space );
}

//
//
//
bool DirSlot::is_empty( ) const
{
    return( m_free_space == std::numeric_limits< std::uint32_t >::max() );
}

//
//
//
void DirSlot::empty()
{
    m_free_space = std::numeric_limits< std::uint32_t >::max();
}
