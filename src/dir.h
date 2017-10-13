#ifndef ROMZDB_DIR_H
#define ROMZDB_DIR_H

#include "buffermgr.h"
#include "pageid.h"
#include <list>
#include "dirpage.h"


class Dir
{
public:
    Dir( BufferMgr& bufferMgr, PageId headerPage );
    ~Dir();

    PageId Insert( std::size_t recLength );

    bool Is( PageId pageId ) const;
    void Delete( PageId pageId, PageOffset freeSpace );

private:
    PageId InsertHeapPage();

private:
    BufferMgr& m_bufferMgr;

    std::list< DirPage >  m_dirPage;
};

#endif
