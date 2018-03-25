#include "frame.h"
#include <stdexcept>
#include <limits>


//
// In the constructor "m_pageId" is initialized to the invalid page ID
//
Frame::Frame()
    : m_pageId( PageId::m_invalid, 0 )
    , m_pinCount( 0 )
    , m_dirty( false )
{

}

//
//
//
PageId Frame::GetPageId( ) const
{
    return m_pageId;
}

//
//
//
bool Frame::IsPinned() const
{
    return ( m_pinCount > 0 );
}

//
//
//
DiskBlock* Frame::GetBlock()
{
    if( m_pinCount == std::numeric_limits< std::uint8_t >::max() )
    {
        throw std::runtime_error( "Frame::GetBlock: Pin count exited the maksimum allowed value." );
    }
    m_pinCount++;
    return &m_block;
}


//
// Reads (from the disk) the page into the frame
//
void Frame::Read( const Space &space, PageId pageId )
{
    if( !pageId.IsValid() )
    {
        throw std::runtime_error( "Frame::Read: Invalid PageId" );
    }

    m_pageId = pageId;
    m_block = space.Read( m_pageId );
    m_dirty = false;
}

//
// Writes (to the disk) the page from the frame
//
void Frame::Write( const Space &space )
{
    if( m_dirty )
    {
        space.Write( m_block, m_pageId );
        m_dirty = false;
    }
}

//
//
//
void Frame::UnpinPage()
{
    if( !IsPinned() )
    {
        throw std::runtime_error( "Frame::UnpinPage: Page is not pinned." );
    }
    m_pinCount--;
}

//
//
//
void Frame::MarkDirty( )
{
    // Page must be pinned to make it diirty
    if( !IsPinned() )
    {
        throw std::runtime_error( "Frame::MarkDirty: Page is not pinned." );
    }
    m_dirty = true;
}

