#include "record.h"
#include <cstring>

//
//
//
Record::Record( const std::vector< char >& data )
    : m_data( data )
{

}

//
//
//
Record::Record( const char* data, std::size_t length )
    : m_data( data, data + length )
{

}

//
//
//
void Record::ToPage( char* page ) const
{
    std::memcpy( page, m_data.data(), m_data.size() );
}

//
//
//
std::size_t Record::GetSize() const
{
    return m_data.size();
}
