#include "page.h"


//
//
//
Page::Page( BufferMgr& bufferMgr, PageId pageId )
    : m_bufferMgr( bufferMgr )
    , m_pageId( pageId )
{
    m_block = m_bufferMgr.pin( pageId );
}

//
//
//
Page::~Page()
{
    m_bufferMgr.unpin( m_pageId, true );
}

//
//
//
Page::Page( const Page& v )
    : m_bufferMgr( v.m_bufferMgr )
    , m_pageId( v.m_pageId )
{
    m_block = m_bufferMgr.pin( m_pageId );
}


//
//
//
const char* Page::GetData() const
{
    return m_block->GetData();
}

//
//
//
char* Page::GetData()
{
    return m_block->GetData();
}


//
//
//
PageId Page::GetPageId( ) const
{
    return m_pageId;
}

