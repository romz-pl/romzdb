#ifndef ROMZDB_RECORDID_H
#define ROMZDB_RECORDID_H

//
// 1. RecordId uniquelly defines the record in the database.
//
// 2. RecordId defines:
//     a) the page, where the record is stored
//     b) the slot on the page, where the record is stored
//


#include "disk/pageid.h"
#include "slotid.h"
#include "util/totally_ordered.h"

class RecordId : public totally_ordered< RecordId >
{
public:
    RecordId( PageId page_id, SlotId slot_id );
    ~RecordId() = default;

    RecordId( const RecordId& v ) = default;
    RecordId& operator=( const RecordId& v ) = default;

    RecordId( RecordId&& v ) = default;
    RecordId& operator=( RecordId&& v ) = default;

    PageId get_page_id() const;
    SlotId get_slot_id() const;

    bool operator==( const RecordId& rid ) const;
    bool operator< ( const RecordId& rid ) const;

private:
    // ID of page on the disk, where the record is stored
    PageId m_page_id;

    // ID of the slot on the page, where the record is stored
    SlotId m_slot_id;
};

#endif
