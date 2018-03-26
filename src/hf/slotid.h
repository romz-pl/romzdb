#ifndef ROMZDB_HF_SLOTID_H
#define ROMZDB_HF_SLOTID_H


//
// Uniquely identifies a record on the page.
//

#include <cstdint>
#include "util/totally_ordered.h"

class SlotId : public totally_ordered< SlotId >
{
private:
    typedef std::uint16_t value_type;

public:
    explicit SlotId( value_type value );
    ~SlotId() = default;

    SlotId( const SlotId& v ) = default;
    SlotId& operator=( const SlotId& v ) = default;

    SlotId( SlotId&& v ) = default;
    SlotId& operator=( SlotId&& v ) = default;

    bool operator==( const SlotId& slotId ) const;
    bool operator< ( const SlotId& slotId ) const;

private:
    value_type m_value;
};


#endif
