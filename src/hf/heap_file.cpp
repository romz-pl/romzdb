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
RecordId HeapFile::insert( const Record& rec )
{
    if( rec.get_length() > HeapPage::GetMaxRecordLength() )
    {
        throw std::runtime_error( "HeapFile::insert: record too long" );
    }


    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );
        auto ret = dp.insert_record( rec );
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
    return insert( rec );
}

//
//
//
void HeapFile::remove( RecordId record_id )
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );
        if( dp.remove_record( record_id ) )
        {
            return;
        }

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            throw std::runtime_error( "HeapFile::remove: no such record id" );
        }
    }
}

//
//
//
Record HeapFile::get( RecordId record_id )
{
    HeapPage hp( m_buffer, record_id.get_page_id() );
    return hp.Get( record_id.get_slot_id() );
}

//
//
//
void HeapFile::alloc_page( )
{

    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );
        if( dp.alloc_page( ) )
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
            new_dir_page.alloc_page( );
            dp.set_next_page( new_dir_page.get_page_id() );
            break;
        }
    }
}


//
//
//
std::uint32_t HeapFile::get_record_no() const
{
    std::uint32_t ret = 0;
    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );

        ret += dp.get_record_no();

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            break;
        }
    }

    return ret;
}

//
//
//
void HeapFile::get_all_records( std::vector< Record >& all ) const
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );

        dp.get_all_records( all );

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            break;
        }
    }
}

//
//
//
void HeapFile::get_all_rids( std::vector< RecordId >& all ) const
{
    PageId dir_page_id = m_header;
    while( true )
    {
        DirPage dp( m_buffer, dir_page_id );

        dp.get_all_rids( all );

        if( dp.is_next_page() )
        {
            dir_page_id = dp.get_next_page();
        }
        else
        {
            break;
        }
    }
}
