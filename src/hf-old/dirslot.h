#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include "disk/pageid.h"
#include "pageoffset.h"
#include "record.h"

class DirSlot
{
public:
    DirSlot( PageId pageId, PageOffset freeSpace );

    bool IsFree( const Record &rec ) const;

public:
    // Page into the "DirSlot" points
    PageId m_pageId;

    // Free space on the "m_pageId" page
    PageOffset m_freeSpace;
};

#endif
