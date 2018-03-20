#ifndef ROMZDB_DB_H
#define ROMZDB_DB_H

#include "pageid.h"
#include <string>
#include "unixfile.h"
#include <memory>
#include "diskspacemgr.h"
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

    std::unique_ptr< UnixFile > m_unixFile;

    std::unique_ptr< DiskSpaceMgr > m_ds;

    std::unique_ptr< BufferMgr > m_bufferMgr;
};

#endif
