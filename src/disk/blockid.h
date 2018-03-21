#ifndef ROMZDB_DISK_BLOCKID_H
#define ROMZDB_DISK_BLOCKID_H

//
// Uniquely identifies the block in unix file.
//

#include <cstdint>

class BlockId
{
public:
    explicit BlockId( std::uint32_t id );
    ~BlockId() = default;

   std::uint32_t GetValue() const;

private:
    std::uint32_t m_id;


};

#endif
