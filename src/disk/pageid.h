#ifndef ROMZDB_DISK_PAGEID_H
#define ROMZDB_DISK_PAGEID_H

//
// Uniquely identifies the logical page.
//

#include <cstdint>

class PageId
{
public:
    explicit PageId( std::uint32_t id );

    PageId( const PageId& ) = default;
    PageId& operator=( const PageId& ) = default;

    PageId( PageId&& ) = default;
    PageId& operator=( PageId&& ) = default;

    std::uint32_t GetValue() const;

    bool IsValid() const;

    bool operator==( const PageId& v ) const;

public:
    static const std::uint32_t m_invalid;

private:
    std::uint32_t m_id;


};

#endif
