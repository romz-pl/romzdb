#include <stdexcept>
#include "frame.h"

//
//
//
Frame::Frame( )
    : m_page_id( 0, 0 )
{
    clear();
}

//
// Initialize buffer frame for a new user
//
void Frame::clear()
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
void Frame::set( PageId page_id )
{
    m_page_id = page_id;
    m_pin_count++;
    m_dirty = false;
    m_valid = true;
    m_refbit = true;
}

//
//
//
void Frame::flush( Space& space )
{
    if( !m_valid )
    {
        return;
    }

    if( m_pin_count > 0)
    {
        throw std::runtime_error( "Frame::flush: Page is pinned" );
    }

    if( m_dirty )
    {
        //flush page to disk
        space.Write( m_block, m_page_id );
        m_dirty = false;
    }
}

//
//
//
void Frame::dispose( Space& space, PageId page_id )
{
    if( !m_valid )
    {
        return;
    }

    if( m_dirty )
    {
        space.Write( m_block, page_id );
    }

    clear();
    space.Dealloc( page_id );
}

//
//
//
void Frame::unpin( bool dirty )
{
    if( m_pin_count > 0 )
    {
        m_pin_count--;
        if( dirty )
        {
            m_dirty = true;
        }
    }
    else
    {
        throw std::runtime_error( "Frame::unpin: Page not pinned" );
    }
}

//
//
//
DiskBlock* Frame::pin()
{
    m_refbit = true;
    m_pin_count++;
    return &m_block;
}

//
//
//
void Frame::write( Space& space )
{
    if( m_dirty )
    {
        space.Write( m_block, m_page_id );
        m_dirty = false;
    }
}

//
//
//
DiskBlock *Frame::read( Space& space, PageId page_id )
{
    m_block = space.Read( page_id );
    set( page_id );
    return &m_block;
}
