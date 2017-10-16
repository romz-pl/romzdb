#ifndef ROMZDB_DIR_H
#define ROMZDB_DIR_H

#include "buffermgr.h"
#include "pageid.h"
#include <list>
#include "dirpage.h"
#include "record.h"
#include "recordid.h"


class Dir
{
public:
    Dir( BufferMgr& bufferMgr, PageId headerPage );
    ~Dir() = default;

    Record Get( RecordId rid ) const;
    RecordId Insert( const Record& rec );
    void Delete( RecordId rid );

private:
    void InsertHeapPage();

private:
    BufferMgr& m_bufferMgr;

    std::list< DirPage >  m_dirPage;
};

#endif
