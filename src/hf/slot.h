#ifndef ROMZDB_SLOT_H
#define ROMZDB_SLOT_H

#include "page.h"
#include "record.h"

class Slot
{
public:
    Slot( std::uint16_t offset, std::uint16_t length );
    ~Slot() = default;

    Record get_record( const char* data ) const;

    void SetInvalid();
    bool IsValid() const;

    std::uint16_t get_length() const;
    std::uint16_t get_offset() const;

    void dec_offset( std::uint16_t v );

private:
    // Record offset
    std::uint16_t m_offset;

    // Record length
    std::uint16_t m_length;

    static const std::uint16_t m_invalid;
};

#endif
