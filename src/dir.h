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
    ~Dir() = default;

    bool Is( PageId pageId ) const;
    PageId Insert();
    void Delete( PageId pageId );

private:
    BufferMgr& m_bufferMgr;

    std::list< DirPage >  m_dirPage;
};

#endif