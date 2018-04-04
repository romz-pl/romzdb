#include "heap_file.h"
#include <string>
#include <stdexcept>
#include <cassert>

//
//
//
HeapFile::HeapFile( BufferMgr& buffer )
    : m_buffer( buffer )
    , m_header( 0, 0 )
{
    auto ret = m_buffer.alloc();
    m_header = ret.first;

    DirPage( ret.second ).init();
    m_buffer.unpin( m_header, true );
}

//
//
//
HeapFile::HeapFile( BufferMgr& buffer, PageId header )
    : m_buffer( buffer )
    , m_header( header )
{

}

//
//
//
PageId HeapFile::get_header_page() const
{
    return m_header;
}

//
//
//
PageId HeapFile::insert( std::uint32_t count )
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DiskBlock* block = m_buffer.pin( dir_page_id );
        DirPage dp( block );
        auto ret = dp.insert_record( count );
        if( ret.has_value() )
        {
            m_buffer.unpin( dir_page_id, true );
            return ret.value();
        }

        if( dp.is_next_page() )
        {
            const PageId prev_dir_page = dir_page_id;
            dir_page_id = dp.get_next_page();
            m_buffer.unpin( prev_dir_page, true );
        }
        else
        {
            m_buffer.unpin( dir_page_id, true );
            break;
        }
    }

    alloc_page();
    return insert( count );
}


//
//
//
void HeapFile::remove( PageId page_id, std::uint32_t count )
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DiskBlock* block = m_buffer.pin( dir_page_id );
        DirPage dp( block );
        if( dp.remove_record( page_id, count ) )
        {
            m_buffer.unpin( dir_page_id, true );
            return;
        }

        if( dp.is_next_page() )
        {
            const PageId prev_dir_page = dir_page_id;
            dir_page_id = dp.get_next_page();
            m_buffer.unpin( prev_dir_page, true );
        }
        else
        {
            m_buffer.unpin( dir_page_id, true );
            throw std::runtime_error( "HeapFile::remove: not removed" );
        }
    }
}

//
//
//
PageId HeapFile::alloc_page( )
{
    const PageId page_id = m_buffer.alloc().first;
    PageId dir_page_id = m_header;
    while( true )
    {
        DiskBlock* block = m_buffer.pin( dir_page_id );
        DirPage dp( block );
        if( dp.alloc_page( page_id ) )
        {
            m_buffer.unpin( dir_page_id, true );
            break;
        }

        if( dp.is_next_page() )
        {
            const PageId prev_dir_page = dir_page_id;
            dir_page_id = dp.get_next_page();
            m_buffer.unpin( prev_dir_page, false );
        }
        else
        {
            auto ret = m_buffer.alloc();
            dp.set_next_page( ret.first );
            DirPage new_dir_page( ret.second );
            new_dir_page.init();
            new_dir_page.alloc_page( page_id );

            m_buffer.unpin( ret.first, true );
            m_buffer.unpin( dir_page_id, true );
            break;
        }
    }

    m_buffer.unpin( page_id, true );
    return page_id;
}

//
//
//
void HeapFile::dispose_page( PageId page_id )
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DiskBlock* block = m_buffer.pin( dir_page_id );
        DirPage dp( block );
        if( dp.dispose_page( page_id ) )
        {
            m_buffer.dispose( page_id );
            m_buffer.unpin( dir_page_id, true );
            return;
        }

        if( dp.is_next_page() )
        {
            const PageId prev_dir_page = dir_page_id;
            dir_page_id = dp.get_next_page();
            m_buffer.unpin( prev_dir_page, false );
        }
        else
        {
            m_buffer.unpin( dir_page_id, false );
            throw std::runtime_error( "HeapFile::free_page: page not found" );
        }
    }
}
