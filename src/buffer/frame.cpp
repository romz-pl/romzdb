#include <stdexcept>
#include "frame.h"

//
//
//
Frame::Frame( )
    : m_pin_count( 0 )
    , m_dirty( false )
    , m_refbit( false )
{

}

//
// Set values of member variables corresponding to assignment of frame to a page in the file.
// Called when a frame in buffer pool is allocated to any page in the file
// through read() or alloc()
//
void Frame::set( )
{
    m_pin_count = 1;
    m_dirty = false;
    m_refbit = true;
}

//
//
//
void Frame::flush( Space& space, PageId page_id )
{
    if( m_pin_count > 0)
    {
        throw std::runtime_error( "Frame::flush: Page is pinned" );
    }

    if( m_dirty )
    {
        //flush page to disk
        space.Write( m_block, page_id );
        m_dirty = false;
    }
}

//
//
//
void Frame::dispose( Space& space, PageId page_id )
{
    space.Dealloc( page_id );
}

//
//
//
void Frame::unpin( bool dirty )
{
    if( m_pin_count == 0 )
    {
        throw std::runtime_error( "Frame::unpin: Page not pinned" );
    }


    m_pin_count--;
    if( dirty )
    {
        m_dirty = true;
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
void Frame::write( Space& space, PageId page_id )
{
    if( m_dirty )
    {
        space.Write( m_block, page_id );
        m_dirty = false;
    }
}

//
//
//
DiskBlock *Frame::read( Space& space, PageId page_id )
{
    m_block = space.Read( page_id );
    set( );
    return &m_block;
}

//
//
//
bool Frame::is_for_replacement( std::uint32_t& countPinned )
{
    if( m_refbit )
    {
        m_refbit = false;
        return false;
    }

    if( m_pin_count > 0 )
    {
        countPinned++;
        return false;
    }

    return true;
}
