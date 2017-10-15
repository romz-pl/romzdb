#include "page.h"


//
//
//
Page::Page( BufferMgr& bufferMgr, PageId pageId, bool multiplePins )
    : m_bufferMgr( bufferMgr )
    , m_pageId( pageId )
{
    m_block = m_bufferMgr.Get( pageId, multiplePins );
}

//
//
//
Page::~Page()
{
    m_bufferMgr.Unpin( m_pageId );
}

//
//
//
Page::Page( const Page& v )
    : m_bufferMgr( v.m_bufferMgr )
    , m_pageId( v.m_pageId )
{
    m_block = m_bufferMgr.Get( m_pageId, true );
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

