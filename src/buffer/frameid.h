#ifndef ROMZDB_DISK_FRAMEID_H
#define ROMZDB_DISK_FRAMEID_H

//
// Uniquely identifies the frame in buffer pool
//

#include <cstdint>
// #include "util/totally_ordered.h"

class FrameId //: public totally_ordered< BlockId >
{
public:
    explicit FrameId( std::uint32_t id );
    ~FrameId() = default;

    FrameId( const FrameId& ) = default;
    FrameId& operator=( const FrameId& ) = default;

    FrameId( FrameId&& ) = default;
    FrameId& operator=( FrameId&& ) = default;

    void inc( std::uint32_t frame_no );

    std::uint32_t to_uint32() const;

    //bool operator==( const BlockId& v ) const;
    //bool operator< ( const BlockId& v ) const;

    //std::uint32_t GetValue() const;

private:
    std::uint32_t m_id;


};

#endif
