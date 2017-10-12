#ifndef ROMZDB_RECORD_H
#define ROMZDB_RECORD_H

//
// 1. Record is the sequence of the bytes.
//
// 2. The record id uniquelly identyfied by record ID
//

#include <vector>
#include <string>

class Record
{
public:
    explicit Record( const std::vector< char >& data );

    explicit Record( const std::string& str );

    Record( const char* data, std::size_t length );

    void ToPage( char* page ) const;

    std::size_t GetLength() const;

    bool operator==( const Record& a ) const;

private:
    std::vector< char > m_data;
};

#endif
