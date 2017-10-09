#include "frame.h"
#include <cassert>
#include <limits>
#include <stdexcept>
#include <iostream>


// Invalid page ID is the maksimum allowed value
const PageId Frame::m_invalidPageId = std::numeric_limits< PageId >::max();

//
// In the constructor "m_pageId" is initialized to the invalid page ID
//
Frame::Frame()
    : m_pageId( m_invalidPageId )
    , m_pinCount( 0 )
    , m_dirty( false )
{

}

//
// Frames are equal, iff the pages ID are equal
//
bool Frame::IsEqual( PageId pageId ) const
{
    return m_pageId == pageId;
}

//
// Reads (from the disk) the page into the frame
//
void Frame::Read( const DiskSpaceMgr &ds, PageId pageId )
{
    if( pageId == m_invalidPageId )
    {
        std::string txt( "Frame::Read: The page ID '" );
        txt += std::to_string( pageId );
        txt += "' is not allowed";
        throw std::runtime_error( txt );
    }

    m_pageId = pageId;
    m_page = ds.Read( m_pageId );
    m_dirty = false;
}

//
// Writes (to the disk) the page from the frame
//
void Frame::Write( const DiskSpaceMgr &ds )
{
    if( m_dirty )
    {
        ds.Write( m_page, m_pageId );
        m_dirty = false;
    }
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

//
//
//
bool Frame::IsPinned() const
{
    return m_pinCount > 0;
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
// Returns pointer to the holded page
//
Page* Frame::GetPage()
{
    if( m_pinCount == std::numeric_limits< std::uint8_t >::max() )
    {
        throw std::runtime_error( "Frame::GetPage: Pin count exited the maksimum allowed value." );
    }
    m_pinCount++;
    return &m_page;
}

//
//
//
void Frame::Print() const
{
    if( m_pageId == m_invalidPageId )
    {
        std::cout << " Uninitialized frame\n";
    }
    else
    {
        std::cout << " page-ID = " << m_pageId << ";";
        std::cout << " is-dirty = " << std::boolalpha << m_dirty << ";";
        std::cout << " pin-count = " << static_cast< int>( m_pinCount ) << ";\n";
    }
}

