#ifndef ROMZDB_DISK_BLOCKID_H
#define ROMZDB_DISK_BLOCKID_H

//
// Uniquely identifies the block in unix file.
//

#include <cstdint>

class BlockId
{
public:
    BlockId();
    explicit BlockId( std::uint32_t id );

   std::uint32_t GetValue() const;

private:
    static const std::uint32_t m_invalid;

private:
    std::uint32_t m_id;


};

#endif
