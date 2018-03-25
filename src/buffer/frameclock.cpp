#include <stdexcept>
#include "frameclock.h"

//
//
//
FrameClock::FrameClock( )
    : m_page_id( 0, 0 )
{
    clear();
}

//
// Initialize buffer frame for a new user
//
void FrameClock::clear()
{
    m_pin_count = 0;
    m_page_id = PageId( 0, 0 );
    m_dirty = false;
    m_refbit = false;
    m_valid = false;
}

//
// Set values of member variables corresponding to assignment of frame to a page in the file.
// Called when a frame in buffer pool is allocated to any page in the file
// through read() or alloc()
//
void FrameClock::set( PageId page_id )
{
    m_page_id = page_id;
    m_pin_count = 1;
    m_dirty = false;
    m_valid = true;
    m_refbit = true;
}

//
//
//
void FrameClock::flush( Space& space )
{
    if( !m_valid )
    {
        return;
    }

    if( m_pin_count > 0)
    {
        throw std::runtime_error( "FrameClock::flush: Page is pinned" );
    }

    if( m_dirty )
    {
        //flush page to disk
        space.Write( m_block, m_page_id );
        m_dirty = false;
    }
}

