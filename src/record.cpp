#include "record.h"
#include <cstring>
#include <random>
#include <algorithm>

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

//
//
//
std::string Record::ToString() const
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
std::string Record::RandomString( )
{
    const std::string alphabet( "0123456789"
                                "abcdefghijklmnopqrstuvwxyz"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

    std::random_device rd;
    std::mt19937 g( rd() );
    std::uniform_int_distribution< std::string::size_type > pick( 0, alphabet.size() );

    std::string::size_type length = pick( g );
    std::string s;
    s.reserve( length );

    while( length-- )
    {
        const char c = alphabet[ pick( g ) ];
        s += c;
    }
    return s;
}
