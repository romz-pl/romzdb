#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include "page.h"
#include <vector>
#include "buffermgr.h"
#include "record.h"
#include "recordid.h"
#include "dirslot.h"


class DirPage : public Page
{
public:
    DirPage( BufferMgr &bufferMgr, PageId self );
    ~DirPage();

    std::pair< bool, Record > Get( RecordId rid ) const;
    std::pair< bool, RecordId > Insert( const Record& rec );
    bool InsertHeapPage( PageId pageId );

    bool Delete( RecordId rid);

    PageId GetNextPage() const;
    void SetNextPage( PageId id );

    void ToPage();
    void FromPage();

    std::size_t GetRecordNo() const;
    void GetRid( std::vector< RecordId >& rid ) const;

private:
    bool IsFull() const;

private:
    PageId m_nextPage;

    std::vector< DirSlot > m_dirSlot;
};

#endif
