#ifndef ROMZDB_HEAPFILE_H
#define ROMZDB_HEAPFILE_H

//
// 1. This class represents the Heap File, the simplest file structure
//
// 2. The data in the pages of Heap File is not ordered in any way.
//
// 3. The anly guarantee is, that one can retrieve all records in the file
//    by repeated requests for the next record.
//
// 4. Every record in the file has a unique record ID.
//
// 5. Every page in a Heap File is of the same size.
//
// 6. The scan of all records in Heap File is implemented in separate
//    class HeapFileScan
//
// 7. The pages in Heap File are maintained by  Directory-of-Pages.
//
// 8. The first page of the directory is passed to the constructor of Heap File class.
//
// 9. The Directory-of-Pages is itself a collection of pages.
//
// 10. Each directory entry identyfies a page in the heap file.
//
// 11. As the Heap File grows or shrinks, the number of entries in the directory
//     (and possibly the number of pages in the directry itself) groows or shrinks
//     correspondingly.
//
// 12. Free space in Heap File is managed by maintaining a "count" per entry, where "count"
//     indicates the amount of free space on the page. The "count" determines if the record
//     will fit on the page pointed to by the entry.
//



#include "pageid.h"
#include "record.h"
#include "recordid.h"
#include "buffermgr.h"
#include "dir.h"

class HeapFile
{
public:
    HeapFile( BufferMgr& bufferMgr, PageId headerPage);
    ~HeapFile() = default;

    Record Get( RecordId rid );
    RecordId Insert( const Record& rec );
    void Delete( RecordId rid );
    void Update( RecordId rid, const Record& rec );

    std::size_t GetRecordNo() const;
    void GetRid( std::vector< RecordId >& rid ) const;

private:
    // Buffer manager
    BufferMgr m_bufferMgr;

    // Directory for Heap file
    Dir m_dir;
};

#endif
