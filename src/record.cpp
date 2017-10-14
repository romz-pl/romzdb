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
void Record::ToPage( char* page ) const
{
    std::memcpy( page, m_data.data(), m_data.size() );
}

//
//
//
PageOffset Record::GetLength() const
{
    return PageOffset( m_data.size() );
}

//
//
//
bool Record::operator==( const Record& a ) const
{
    return m_data == a.m_data;
}
