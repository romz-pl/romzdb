#ifndef ROMZDB_DB_H
#define ROMZDB_DB_H

#include "pageid.h"
#include <string>
#include "unixfile.h"
#include "space.h"
#include "buffermgr.h"
#include "heapfile.h"

class Db
{
public:
    Db( const std::string& path , std::size_t frameNo );
    ~Db() = default;

    HeapFile CreteHeapFile();

private:
    void CreateHeaderPage( PageId headerPage );

private:

    Space m_space;

    BufferMgr m_bufferMgr;
};

#endif
