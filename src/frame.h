#ifndef ROMZDB_FRAME_H
#define ROMZDB_FRAME_H

//
// It contains the data about a page in the buffer manager.
//


#include "pageid.h"
#include "page.h"
#include "unixfile.h"

class Frame
{
public:
    Frame( Page* page, PageId pageId, UnixFile* uf );
    void Write();
    void Read();
    // void Init( UnixFile* uf, PageId pageId );

    void SetDirty( bool dirty );

private:
    // Page contents
    Page* m_page;

    // Page identyfier for this page
    PageId m_pageId;

    // Unix file, where the page is stored
    UnixFile* m_uf;

    // pin count
    short int m_pinCount;

    // true if page is dirty
    bool m_dirty;

};

#endif
