#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include "pageid.h"
#include "pageoffset.h"

class DirSlot
{
public:
    PageId m_pageId;
    PageOffset m_freeSpace;
};

#endif
