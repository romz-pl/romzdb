#include "page.h"


//
//
//
Page::Page( BufferMgr& bufferMgr, PageId pageId, bool multiplePins )
    : m_bufferMgr( bufferMgr )
    , m_pageId( pageId )
{
    m_block = m_bufferMgr.GetPage( pageId, multiplePins );
}

//
//
//
Page::~Page()
{
    m_bufferMgr.UnpinPage( m_pageId );
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

