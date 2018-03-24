#ifndef ROMZDB_DISK_BLOCKID_H
#define ROMZDB_DISK_BLOCKID_H

//
// Uniquely identifies the block in unix file.
//

#include <cstdint>
#include "util/totally_ordered.h"

class BlockId : public totally_ordered< BlockId >
{
public:
    explicit BlockId( std::uint32_t id );
    ~BlockId() = default;

    BlockId( const BlockId& ) = default;
    BlockId& operator=( const BlockId& ) = default;

    BlockId( BlockId&& ) = default;
    BlockId& operator=( BlockId&& ) = default;

    bool operator==( const BlockId& v ) const;
    bool operator< ( const BlockId& v ) const;

    std::uint32_t GetValue() const;

private:
    std::uint32_t m_id;


};

#endif
