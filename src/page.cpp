#include "page.h"
#include <cstring>
#include <cassert>
#include <algorithm>
#include <limits>

static_assert( std::numeric_limits< PageOffset >::max() > Page::PageSize, "Two small size" );


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
    const std::size_t len = std::min( static_cast< std::size_t >( PageSize ), v.size() );
    for( std::size_t i = 0; i < len; i++ )
        m_data[ i ] = v[ i ];
}

//
// Zeroing the data
//
void Page::Zero( )
{
    std::fill( m_data.begin(), m_data.end(), 0 );
}

//
// Writes the page to file on pageId
//
void Page::Write( const UnixFile& uf, PageId pageId ) const
{
    const off_t offset = pageId * (off_t)PageSize;
    uf.Write( m_data.data(), PageSize, offset );
}

//
// Reads the page from file from pageId
//
void Page::Read( const UnixFile &uf, PageId pageId )
{
    const off_t offset = pageId * (off_t)PageSize;
    uf.Read( m_data.data(), PageSize, offset );
}

//
//
//
bool Page::operator==( const Page& a ) const
{
    return m_data == a.m_data;
}

//
//
//
const char* Page::GetData() const
{
    return m_data.data();
}

//
//
//
char* Page::GetData()
{
    return m_data.data();
}

