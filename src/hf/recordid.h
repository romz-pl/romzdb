#ifndef ROMZDB_RECORDID_H
#define ROMZDB_RECORDID_H

//
// 1. RecordId uniquelly defines the record in the database.
//
// 2. RecordId defines:
//     a) the page, where the record is stored
//     b) the slot on the page, where the record is stored
//


#include "pageid.h"
#include "slotid.h"

class RecordId
{
public:
    RecordId( PageId pageId, SlotId slotId );

    PageId GetPageId() const;
    SlotId GetSlotId() const;

    bool operator == ( const RecordId& rid ) const;

private:
    // ID of page on the disk, where the record is stored
    PageId m_pageId;

    // ID of the slot on the page, where the record is stored
    SlotId m_slotId;
};

#endif
