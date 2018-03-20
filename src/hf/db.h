#ifndef ROMZDB_DB_H
#define ROMZDB_DB_H

#include "pageid.h"
#include <string>
#include "unixfile.h"
#include <memory>
#include "disk.h"
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

    Disk m_disk;

    BufferMgr m_bufferMgr;
};

#endif
