#include "page.h"
#include <cstring>
#include <cassert>
#include <algorithm>

//
//
//
Page::Page()
{
    Zero();
}

//
//
//
Page::Page( const std::string& v )
{
    const std::size_t len = std::min( m_size, v.size() );
    std::memcpy( m_data, &( v[ 0 ]), len );
}

//
// Zeroing the data
//
void Page::Zero( )
{
    std::memset( m_data, 0, m_size );
}

//
// Writes the page to file on pageId
//
void Page::Write( const UnixFile& uf, PageId pageId ) const
{
    const off_t offset = pageId * (off_t)m_size;
    uf.Write( m_data, m_size, offset );
}

//
// Reads the page from file from pageId
//
void Page::Read( const UnixFile &uf, PageId pageId )
{
    const off_t offset = pageId * (off_t)m_size;
    uf.Read( m_data, m_size, offset );
}

//
//
//
bool Page::operator==( const Page& a ) const
{
    return std::memcmp( m_data, a.m_data, m_size ) == 0;
}
