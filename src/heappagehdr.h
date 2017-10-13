#ifndef ROMZDB_HEAPPAGEHDR_H
#define ROMZDB_HEAPPAGEHDR_H

#include "page.h"
#include <vector>
#include "slot.h"
#include "slotid.h"
#include "record.h"

//
// 1. This is Header of Heap Page.
//
// 2. The Header keeps track of the organiation of the records on Heap Page.
//
// 3. The records can be:
//     a) fixed length,
//     b) variable length
//
// 4. The Header holds the array of Slots
//
// 5. Inserting the record is done by adding new entry at the end of the array of slots.
//
// 6. Deleting the slots is done by erasing the element from the array of slots,
//    and moving the offset of all remaing record on the page.
//
// 7. Inserting and Deleting procedures keeps the occupied space on the page
//    without holes (gaps).
//

class HeapPageHdr
{
public:
    HeapPageHdr() = default;
    ~HeapPageHdr() = default;

    void ToPage( Page& page ) const;
    void FromPage( const Page& page );

    std::size_t GetSlotNo() const;
    Slot GetSlot( SlotId slotId ) const;

    std::pair<PageOffset, SlotId> Insert( std::size_t recLength );
    PageOffset Delete( SlotId slotId );

    std::int32_t GetFreeSpace() const;


private:
    std::vector< Slot > m_slot;
};

#endif
