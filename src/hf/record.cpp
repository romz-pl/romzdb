#include "record.h"
#include <cstring>

/*
 *
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
Record::Record( const std::string& str )
    : m_data( str.c_str(), str.c_str() + str.size() )
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
void Record::copy_to_page( void* page ) const
{
    std::memcpy( page, m_data.data(), m_data.size() );
}

//
//
//
std::uint32_t Record::get_length() const
{
    return static_cast< std::uint32_t >( m_data.size() );
}

//
//
//
std::string Record::to_string() const
{
    std::string str;
    for( char v : m_data )
        str += v;
    // std::copy( m_data.begin(), m_data.end(), str.begin() );
    return str;
}

//
//
//
bool Record::operator==( const Record& a ) const
{
    return m_data == a.m_data;
}

//
//
//
bool Record::operator< (const Record& a ) const
{
    return m_data < a.m_data;
}

*/
