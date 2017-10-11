#ifndef ROMZDB_PAGE_H
#define ROMZDB_PAGE_H

//
// Represents "the page of data".
// The page can be stored on the disk or hold in the memory
//

#include "pageid.h"
#include "unixfile.h"
#include <string>
#include <array>
#include <cstring>
#include <stdexcept>


using PageOffset = std::uint16_t;


class Page
{
public:
    // The size of the page in the file.
    // All pages have the same size!
    enum { PageSize = 4096 };


public:
    Page();
    explicit Page( const std::string& v );

    void Write( const UnixFile& uf, PageId pageId ) const;
    void Read( const UnixFile& uf, PageId pageId );

    bool operator==( const Page& a ) const;

    const char* GetData() const;
    char* GetData();

    template< typename T >
    void Set( T value, PageOffset offset );

    template< typename T >
    T Get( PageOffset offset ) const;

private:
    void Zero( );

    template< typename T >
    void CheckOffset( PageOffset offset ) const;

private:
    // Data stored on the page
    std::array< char, PageSize > m_data;
};

//
//
//
template< typename T >
void Page::Set( T value, PageOffset offset )
{
    CheckOffset< T >( offset );
    std::memcpy( m_data.data() + offset, &value, sizeof( T ) );
}

//
//
//
template< typename T >
T Page::Get( PageOffset offset ) const
{
    T tmp = 0;
    CheckOffset< T >( offset );
    std::memcpy( &tmp, m_data.data() + offset, sizeof( T ) );
    return tmp;
}

//
//
//
template< typename T >
void Page::CheckOffset( PageOffset offset ) const
{
    if( offset + sizeof( T ) > m_data.size() )
    {
        throw std::runtime_error( "Page::CheckOffset: Offset too large.");
    }
}

#endif
