#ifndef ROMZDB_PAGE_H
#define ROMZDB_PAGE_H

//
// Represents "the page of data".
// The page can be stored on the disk or hold in the memory
//


class Page
{
public:
    Page();

    void Zero( );

    static int GetSize();

private:
    // The size of the page in the file.
    // All pages have the same size!
    static const int m_size = 4096;

    // Data stored on the page
    char m_data[ m_size ];
};


#endif
