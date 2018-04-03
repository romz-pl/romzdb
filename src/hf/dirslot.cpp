#include <limits>
#include <stdexcept>
#include "dirslot.h"
#include "disk/diskblock.h"

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

//
//
//
bool DirSlot::insert_record( std::uint32_t count )
{
    if( is_empty( ) )
    {
        return false;
    }

    if( is_free( count ) )
    {
        m_free_space -= count;
        return true;
    }

    return false;
}

//
//
//
bool DirSlot::remove_record( PageId page_id, std::uint32_t count )
{
    if( is_empty( ) )
    {
        return false;
    }

    if( m_page_id != page_id )
    {
        return false;
    }

    m_free_space += count;
    return true;
}

//
//
//
bool DirSlot::add_page( PageId page_id )
{
    if( is_empty( ) )
    {
        m_page_id = page_id;
        m_free_space = DiskBlock::Size;
        return true;
    }

    return false;
}

//
//
//
bool DirSlot::free_page( PageId page_id )
{
    if( is_empty( ) )
    {
        return false;
    }

    if( m_page_id != page_id )
    {
        return false;
    }

    empty();
    return true;

}

//
//
//
PageId DirSlot::get_page_id() const
{
    return m_page_id;
}
