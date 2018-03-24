#include "pageid.h"
#include <limits>

//
//
//
const std::uint32_t PageId::m_invalid = std::numeric_limits< std::uint32_t >::max();



//
//
//
PageId::PageId( std::uint32_t block_id, std::uint32_t db_file_id )
    : m_block_id( block_id )
    , m_db_file_id( db_file_id )
{

}

//
//
//
PageId::PageId( BlockId block_id, DbFileId db_file_id )
    : m_block_id( block_id )
    , m_db_file_id( db_file_id )
{

}

//
//
//
BlockId PageId::get_block_id() const
{
    return m_block_id;
}

//
//
//
DbFileId PageId::get_db_file_id() const
{
    return m_db_file_id;
}


//
//
//
bool PageId::IsValid() const
{
    return ( m_block_id.GetValue() != m_invalid );
}



//
//
//
bool PageId::operator== ( const PageId& v ) const
{
    return ( m_block_id == v.m_block_id ) && ( m_db_file_id == v.m_db_file_id );
}

//
//
//
bool PageId::operator< ( const PageId& v ) const
{
    if( m_block_id == v.m_block_id )
    {
        return ( m_db_file_id < v.m_db_file_id );
    }

    return ( m_block_id < v.m_block_id );
}

