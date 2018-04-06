#ifndef ROMZDB_HF_RECORD_H
#define ROMZDB_HF_RECORD_H

//
// 1. Record is the sequence of the bytes.
//
// 2. The record is uniquelly identyfied by record ID
//

#include <vector>
#include <string>

class Record
{

public:
    explicit Record( const std::vector< char >& data );
    explicit Record( const std::string& str );
    Record( const char* data, std::size_t length );

    void copy_to_page( void* page ) const;

    std::uint32_t get_length() const;
    std::string to_string() const;

    bool operator==( const Record& a ) const;
    bool operator< (const Record& rec ) const;

private:
    std::vector< char > m_data;

};

#endif
