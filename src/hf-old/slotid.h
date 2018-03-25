#ifndef ROMZDB_SLOTID_H
#define ROMZDB_SLOTID_H


//
// Uniquely identifies a record on the page.
//

#include <cstdint>


class SlotId
{
public:
    explicit SlotId( std::uint16_t value );

    std::uint16_t GetValue() const;

    bool operator ==( const SlotId& slotId ) const;

private:
    std::uint16_t m_value;
};


#endif
