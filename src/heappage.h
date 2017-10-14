#ifndef ROMZDB_HEAPPAGE_H
#define ROMZDB_HEAPPAGE_H

//
// 1. Heap Page is the page in the Heap File.
//
// 2. Heap page is the collection of SLOTS, each of which contains the record.
//
// 3. The records in the heap page are variable-lengths.
//
// 4. The slots on the heap page are maintained by Directory of Slots,
//    with <record-offset, record-length> pair per slot.
//
// 5. The record-offset is a "pointer" to the record, i.e. it is the offset
//    in bytes from the start of the data on the page to the start of the record.
//
// 6. Deletion of the record is readily accoplished by setting the record-offset to (-1);
//
// 7. SlotId defines the position in the directory.
//
// 8. Record can be moved around on the page because the RecordId (which is the PageId, SlotId)
//    does not change when the record is moved. Only the record-offset (stored in the slot) changes.
//
// 9. Exerpt from book "Database management System" by R.Ramakrishnan and J. Gehrke
//    Third edition, Chapter 9.6.2 (Storing Data: Disk and Files / Page Formats / Variable-Length Records)
//
//   The space available for new records must be managed carefully because the page
//   is not preformatted into slots. One way to manage free space is to maintain a
//   pointer (that is, offset from the start of the data area on the page) that indicates
//   the start of the free space area. When a new record is too large to fit into the
//   remaining free space, we have to move records on the page to reclaim the space
//   freed by records deleted earlier. The idea is to ensure that, after reorganization,
//   all records appear in contiguous order, followed by the available free space.
//
//   A subtle point to be noted is that the slot for a deleted record cannot always
//   be removed from the slot directory, because slot numbers are used to identify
//   records -- by deleting a slot, we change (decrement) the slot number of subsequent
//   slots in the slot directory, and thereby change the rid of records pointed
//   to by subsequent slots. The only way to remove slots from the slot directory is
//   to remove the last slot if the record that it points to is deleted. However, when
//   a record is inserted, the slot directory should be scanned for an element that
//   currently does not point to any record, and this slot should be used for the new
//   record. A new slot is added to the slot directory only if all existing slots point
//   to records. If inserts are much more common than deletes (as is typically the
//   case), the number of entries in the slot directory is likely to be very close to
//   the actual number of records on the page.
//
//  End-of-exerpt
//

#include "slotid.h"
#include "slot.h"
#include "page.h"
#include "record.h"
#include "buffermgr.h"
#include <vector>
#include "pageoffset.h"

class HeapPage : public Page
{

public:
    HeapPage( BufferMgr& bufferMgr, PageId pageId );
    ~HeapPage();

    Record Get( SlotId slotId );
    SlotId Insert( const Record& rec );
    PageOffset Delete( SlotId slotId );

    std::size_t GetRecordNo() const;

    std::int32_t GetFreeSpace() const;

private:
    std::size_t GetSlotNo() const;
    Slot GetSlot( SlotId slotId ) const;


private:
    void ToPage();
    void FromPage( );

private:
    std::vector< Slot > m_slot;

};

#endif
