#include <limits>
#include <stdexcept>
#include "dirslot.h"
#include "disk/diskblock.h"

//
//
//
DirSlot::DirSlot(  )
    : m_empty( true )
    , m_page_id( 0, 0 )
    , m_free_space( 0 )
{

}


//
// Returns "true" if there is enought free space on "m_pageId"
//
bool DirSlot::is_free( std::uint32_t count ) const
{
    return ( m_free_space >= count );
}

//
//
//
void DirSlot::make_empty()
{
    m_empty = true;
}

//
//
//
bool DirSlot::insert_record( std::uint32_t count )
{
    if( m_empty )
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
    if( m_empty )
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
    if( m_empty )
    {
        m_page_id = page_id;
        m_free_space = DiskBlock::Size;
        m_empty = false;
        return true;
    }

    return false;
}

//
//
//
bool DirSlot::free_page( PageId page_id )
{
    if( m_empty )
    {
        return false;
    }

    if( m_page_id != page_id )
    {
        return false;
    }

    m_empty = true;
    return true;

}

//
//
//
PageId DirSlot::get_page_id() const
{
    return m_page_id;
}
