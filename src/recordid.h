#ifndef ROMZDB_RECORDID_H
#define ROMZDB_RECORDID_H


#include "pageid.h"
#include "slotid.h"

class RecordId
{
public:
    RecordId();

private:
    // ID of page on the disk, where the record is stored
    PageId m_pageId;

    // ID of the slot on the page, where the record is stored
    SlotId m_slotId;
};

#endif
