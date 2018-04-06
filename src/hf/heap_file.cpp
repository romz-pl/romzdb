#include <stdexcept>
#include "heap_file.h"
#include "heappage.h"

//
//
//
HeapFile::HeapFile( BufferMgr& buffer )
    : m_buffer( buffer )
    , m_header( 0, 0 )
{
    DirPage dp( buffer ); // Initialize the first page in directory
    m_header = dp.get_page_id();
}

//
//
//
HeapFile::HeapFile( BufferMgr& buffer, PageId header_page_id )
    : m_buffer( buffer )
    , m_header( header_page_id )
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
void HeapFile::insert( const Record& rec )
{
    const PageId page_id = insert_into_dir( rec.get_length() );
    HeapPage hp( m_buffer, page_id );
    hp.Insert( rec );
}

//
//
//
PageId HeapFile::insert_into_dir( std::uint32_t count )
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );
        auto ret = dp.insert_record( count );
        if( ret.has_value() )
        {
            return ret.value();
        }

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            break;
        }
    }

    alloc_page();
    return insert_into_dir( count );
}


//
//
//
void HeapFile::remove_from_dir( PageId page_id, std::uint32_t count )
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );
        if( dp.remove_record( page_id, count ) )
        {
            return;
        }

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
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
    m_buffer.unpin( page_id, true );

    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );
        if( dp.alloc_page( page_id ) )
        {
            break;
        }

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            DirPage new_dir_page( m_buffer );
            new_dir_page.alloc_page( page_id );
            dp.set_next_page( new_dir_page.get_page_id() );
            break;
        }
    }

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
        DirPage dp( m_buffer, dir_page_id );
        if( dp.dispose_page( page_id ) )
        {
            m_buffer.dispose( page_id );
            return;
        }

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            throw std::runtime_error( "HeapFile::free_page: page not found" );
        }
    }
}
