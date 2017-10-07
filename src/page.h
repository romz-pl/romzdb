#ifndef ROMZDB_PAGE_H
#define ROMZDB_PAGE_H

//
// Represents "the page of data".
// The page can be stored on the disk or hold in the memory
//

#include "pageid.h"
#include "unixfile.h"
#include <string>

class Page
{
public:
    Page();
    explicit Page( const std::string& v );

    void Write( const UnixFile& uf, PageId pageId ) const;
    void Read( const UnixFile& uf, PageId pageId );

    bool operator==( const Page& a ) const;

private:
    void Zero( );

private:
    // The size of the page in the file.
    // All pages have the same size!
    static const std::size_t m_size = 4096;

    // Data stored on the page
    char m_data[ m_size ];
};


#endif
