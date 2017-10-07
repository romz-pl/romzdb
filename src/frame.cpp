#include "frame.h"
#include <cassert>

//
//
//
Frame::Frame()
    : m_pinCount( 0 )
    , m_dirty( false )
{

}

//
//
//
void Frame::Read( const DiskSpaceMgr &ds, PageId pageId )
{
    m_pageId = pageId;
    m_page = ds.Read( m_pageId );
    m_pinCount = 1;
    m_dirty = false;
}

//
//
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
void Frame::SetDirty( bool dirty )
{
    m_dirty = dirty;
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
void Frame::IncPin()
{
    m_pinCount++;
}

//
//
//
void Frame::DecPin()
{
    assert( m_pinCount > 0 );
    m_pinCount--;
}

Page* Frame::GetPage()
{
    return &m_page;
}
