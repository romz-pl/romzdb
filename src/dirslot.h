#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include "pageid.h"
#include "pageoffset.h"

class DirSlot
{
public:
    DirSlot( PageId pageId, PageOffset freeSpace );

public:
    // Page into the "DirSlot" points
    PageId m_pageId;

    // Free space on the "m_pageId" page
    PageOffset m_freeSpace;
};

#endif
