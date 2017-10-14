#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include "page.h"
#include <vector>
#include "buffermgr.h"
#include "record.h"
#include "recordid.h"

class DirSlot
{
public:
    PageId m_pageId;
    std::size_t m_freeSpace;
};

class DirPage : public Page
{
public:
    DirPage( BufferMgr &bufferMgr, PageId self );
    ~DirPage();

    bool IsFull() const;
    bool Is( PageId pageId ) const;

    std::pair< bool, Record > Get( RecordId rid ) const;
    std::pair< bool, RecordId > Insert( const Record& rec );
    void InsertPage( PageId pageId );

    bool Delete( RecordId rid);

    PageId GetNextPage() const;
    void SetNextPage( PageId id );

    PageId GetPageId() const;

private:
    void ToPage();
    void FromPage();

private:
    PageId m_nextPage;

    std::vector< DirSlot > m_dirSlot;
};

#endif
