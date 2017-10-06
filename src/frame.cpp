#include "frame.h"

//
//
//
Frame::Frame( Page* page, PageId pageId, UnixFile* uf )
    : m_page( page )
    , m_pageId( pageId )
    , m_uf( uf )
    , m_pinCount( 0 )
    , m_dirty( false )
{

}

//
//
//
void Frame::Write()
{
    if( m_dirty )
    {
        m_page->Write( m_uf, m_pageId );
        m_dirty = false;
    }
}

//
//
//
void Frame::Read( )
{
    m_page->Read( m_uf, m_pageId );
    m_pinCount = 1;
    m_dirty = false;
}

//
//
//
void Frame::SetDirty( bool dirty )
{
    m_dirty = dirty;
}

/*
//
//
//
void Frame::Init( UnixFile* uf, PageId pageId )
{
    m_uf = uf;
    m_pageId = pageId;
    m_pinCount = 1;
    m_dirty = false;
    m_page.Zero();
}
*/
