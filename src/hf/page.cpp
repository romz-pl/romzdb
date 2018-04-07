#include "page.h"


//
//
//
Page::Page( BufferMgr& buffer )
    : m_buffer( buffer )
    , m_dirty( false )
    , m_page_id( 0, 0 )
    , m_block( nullptr )
{
    auto ret = m_buffer.alloc();
    m_page_id = ret.first;
    m_block = ret.second;
}


//
//
//
Page::Page( BufferMgr& buffer, PageId page_id )
    : m_buffer( buffer )
    , m_dirty( false )
    , m_page_id( page_id )
    , m_block( buffer.pin( page_id ) )
{

}

//
//
//
Page::~Page()
{
    m_buffer.unpin( m_page_id, m_dirty );
}

//
//
//
PageId Page::get_page_id( ) const
{
    return m_page_id;
}

//
//
//
const char* Page::get_data() const
{
    return m_block->GetData();
}

//
//
//
char* Page::get_data()
{
    return m_block->GetData();
}

