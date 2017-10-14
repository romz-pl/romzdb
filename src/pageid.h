#ifndef ROMZDB_PAGEID_H
#define ROMZDB_PAGEID_H

//
// Uniquely identifies the logical page.
//

#include <cstddef>

class PageId
{
public:
    explicit PageId( std::size_t id );

    std::size_t GetValue() const;

    bool IsValid() const;

    bool operator==( const PageId& v ) const;

public:
    static const std::size_t m_invalid;

private:
    std::size_t m_id;


};

#endif
