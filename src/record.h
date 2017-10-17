#ifndef ROMZDB_RECORD_H
#define ROMZDB_RECORD_H

//
// 1. Record is the sequence of the bytes.
//
// 2. The record id uniquelly identyfied by record ID
//

#include <vector>
#include <string>
#include "pageoffset.h"

class Record
{
public:
    explicit Record( const std::vector< char >& data );
    explicit Record( const std::string& str );
    Record( const char* data, std::size_t length );

    void ToPage( char* page ) const;
    PageOffset GetLength() const;

    bool operator==( const Record& a ) const;

    std::string ToString() const;

    static std::string RandomString( );

    bool operator< (const Record& rec ) const;

private:
    std::vector< char > m_data;
};

#endif
