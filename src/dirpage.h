#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include "page.h"
#include <vector>

class DirSlot
{
public:
    PageId m_pageId;
    std::size_t m_freeSpace;
};

class DirPage
{
public:
    DirPage( Page &page, PageId self );
    ~DirPage() = default;

    bool IsFull() const;
    bool Is( PageId pageId ) const;

    std::pair< bool, PageId > InsertRec( std::size_t recLength );
    void InsertPage( PageId pageId );

    bool Delete( PageId pageId, PageOffset freeSpace );

    PageId GetNextPage() const;
    void SetNextPage( PageId id );

    PageId GetPageId() const;

private:
    void ToPage() const;
    void FromPage();

private:
    const PageId m_self;

    PageId m_nextPage;
    std::vector< DirSlot > m_dirSlot;
    Page& m_page;

    static const std::size_t m_maxEntries;
};

#endif
