#ifndef ROMZDB_DB_H
#define ROMZDB_DB_H

#include "disk/pageid.h"
#include <string>
#include "disk/unixfile.h"
#include "disk/space.h"
#include "buffer/buffermgr.h"
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
    DbFile m_db_file;

    Space m_space;

    BufferMgr m_bufferMgr;
};

#endif
