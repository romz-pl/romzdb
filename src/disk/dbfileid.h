#ifndef ROMZDB_DISK_DBFILEID_H
#define ROMZDB_DISK_DBFILEID_H

//
// Uniquely identifies the DB file.
//

#include <cstdint>
#include "totally_ordered.h"

class DbFileId : public totally_ordered< DbFileId >
{
public:
    explicit DbFileId( std::uint32_t id );

    DbFileId( const DbFileId& ) = default;
    DbFileId& operator=( const DbFileId& ) = default;

    DbFileId( DbFileId&& ) = default;
    DbFileId& operator=( DbFileId&& ) = default;

    bool operator==( const DbFileId& v ) const;
    bool operator< ( const DbFileId& v ) const;

private:
    std::uint32_t m_id;


};

#endif
