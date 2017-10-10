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
// 5. Every page in a Heap File is of the same size
//

#include "pageid.h"
#include "record.h"
#include "recordid.h"

class HeapFile
{
public:
public:
    enum class Mode{ Open, Create };
public:
    HeapFile( PageId headerPage, Mode mode );
    ~HeapFile() = default;

    Record Get( RecordId rid );
    void Insert( RecordId rid, const Record& rec );
    void Delete( RecordId rid );
    void Update( RecordId rid, const Record& rec );


private:
    void Open( );
    void Create( );

private:
    const PageId m_headerPage;
};

#endif
