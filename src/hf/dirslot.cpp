#include <limits>
#include <stdexcept>
#include "dirslot.h"
#include "disk/diskblock.h"



const std::uint32_t DirSlot::m_max_free_space = DiskBlock::Size;

/*
//
//
//
DirSlot::DirSlot(  )
    : m_valid( false )
    , m_page_id( 0, 0 )
    , m_free_space( 0 )
{

}
*/


//
//
//
void DirSlot::make_invalid()
{
    m_valid = false;
}

//
//
//
bool DirSlot::insert_record( std::uint32_t count )
{
    if( !m_valid )
    {
        return false;
    }

    if( m_free_space >= count )
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
    if( !m_valid )
    {
        return false;
    }

    if( m_page_id != page_id )
    {
        return false;
    }

    const std::uint32_t space_after_remove = m_free_space + count;

    if( space_after_remove > m_max_free_space )
    {
        throw std::runtime_error( "DirSlot::remove_record: More space removed than allocated" );
    }

    m_free_space = space_after_remove;
    return true;
}

//
//
//
bool DirSlot::alloc_page( PageId page_id )
{
    if( !m_valid )
    {
        m_page_id = page_id;
        m_free_space = m_max_free_space;
        m_valid = true;
        return true;
    }

    return false;
}

//
//
//
bool DirSlot::dispose_page( PageId page_id )
{
    if( !m_valid )
    {
        return false;
    }

    if( !is_empty() )
    {
        throw std::runtime_error( "DirSlot::dispose_page: The page is not empty" );
    }

    if( m_page_id != page_id )
    {
        return false;
    }

    m_valid = false;
    return true;

}

//
//
//
PageId DirSlot::get_page_id() const
{
    return m_page_id;
}

//
//
//
bool DirSlot::is_empty() const
{
    return m_free_space == m_max_free_space;
}
