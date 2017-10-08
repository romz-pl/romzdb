#include "frame.h"
#include <cassert>
#include <limits>

const PageId Frame::m_reservedPageId = std::numeric_limits< PageId >::max();

//
//
//
Frame::Frame()
    : m_pageId( m_reservedPageId )
    , m_pinCount( 0 )
    , m_dirty( false )
{

}

bool Frame::IsEqual( PageId pageId ) const
{
    return m_pageId == pageId;
}

//
//
//
void Frame::Read( const DiskSpaceMgr &ds, PageId pageId )
{
    m_pageId = pageId;
    m_page = ds.Read( m_pageId );
    m_dirty = false;
}

//
//
//
void Frame::Write( const DiskSpaceMgr &ds )
{
    if( m_dirty )
    {
        assert( m_pageId != m_reservedPageId );
        ds.Write( m_page, m_pageId );
        m_dirty = false;
    }
}

//
//
//
void Frame::SetDirty( bool dirty )
{
    m_dirty = dirty;
}

//
//
//
void Frame::MarkDirty( )
{
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
    assert( m_pinCount > 0 );
    m_pinCount--;
}

Page* Frame::GetPage()
{
    m_pinCount++;
    return &m_page;
}
