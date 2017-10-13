#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include "page.h"
#include <vector>

class DirPage
{
public:
    DirPage( Page &page );
    ~DirPage() = default;

    bool IsFull() const;
    bool Is( PageId pageId ) const;

    void Insert( PageId pageId );
    bool Delete( PageId pageId );

    PageId GetNextPage() const;
    void SetNextPage( PageId id );

private:
    void ToPage() const;
    void FromPage();

private:
    PageId m_nextPage;
    std::vector< PageId > m_pageId;
    Page& m_page;

    static const std::size_t m_maxEntries;
};

#endif
