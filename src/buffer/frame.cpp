#include <stdexcept>
#include "frame.h"

//
//
//
Frame::Frame( )
    : m_page_id( 0, 0 )
    , m_pin_count( 0 )
    , m_dirty( false )
    , m_valid( false )
    , m_refbit( false )
{

}

//
// Set values of member variables corresponding to assignment of frame to a page in the file.
// Called when a frame in buffer pool is allocated to any page in the file
// through read() or alloc()
//
void Frame::set( PageId page_id )
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
void Frame::flush( Space& space )
{
    if( !m_valid )
    {
        throw std::runtime_error( "Frame::flush: Invalid frame" );
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
        throw std::runtime_error( "Frame::dispose: Invalid frame" );
    }

    if( m_dirty )
    {
        space.Write( m_block, page_id );
    }

    space.Dealloc( page_id );
    m_valid = false;
}

//
//
//
void Frame::unpin( bool dirty )
{
    if( !m_valid )
    {
        throw std::runtime_error( "Frame::unpin: Invalid frame" );
    }

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
    if( !m_valid )
    {
        throw std::runtime_error( "Frame::pin: Invalid frame" );
    }

    m_refbit = true;
    m_pin_count++;
    return &m_block;
}

//
//
//
void Frame::write( Space& space )
{
    if( !m_valid )
    {
        throw std::runtime_error( "Frame::write: Invalid frame" );
    }

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

//
//
//
bool Frame::is_for_replacement( Space& space, std::map< PageId, Frame* >& map, std::uint32_t& countPinned )
{
    if( !m_valid )
    {
        return true;
    }

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

    write( space );
    map.erase( m_page_id );
    m_valid = false;
    return true;
}

//
//
//
bool Frame::is_valid() const
{
    return m_valid;
}
