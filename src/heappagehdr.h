#ifndef ROMZDB_HEAPPAGEHDR_H
#define ROMZDB_HEAPPAGEHDR_H

#include "page.h"
#include <vector>
#include "slot.h"
#include "slotid.h"
#include "record.h"

class HeapPageHdr
{
public:
    HeapPageHdr();
    ~HeapPageHdr() = default;

    void ToPage( Page& page ) const;
    void FromPage( const Page& page );

    std::size_t GetSlotNo() const;
    Slot GetSlot( SlotId slotId ) const;

    PageOffset Insert( const Record& rec );
    void Delete( SlotId slotId );


private:
    PageOffset m_freeSpace;

    std::vector< Slot > m_slot;
};

#endif
