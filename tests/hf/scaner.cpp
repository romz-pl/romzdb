#include <gtest/gtest.h>
#include <heapfile.h>
#include <db.h>
#include <scaner.h>
#include <random>
#include <algorithm>
#include "temp_path.h"


std::vector< Record > GenerateData()
{
    std::size_t recordNo = 1234;
    std::vector< Record > ret;
    ret.reserve( recordNo );
    std::string txt;

    for( std::size_t i = 0; i < recordNo; i++ )
    {
        txt = Record::RandomString();
        ret.push_back( Record( txt ) );
    }

    return ret;

}

TEST(Scaner, Compare)
{
    const std::string path = GetTempPath();
    const std::size_t frameNo = 50;

    Db db( path, frameNo );
    HeapFile hf = db.CreteHeapFile();

    std::vector< Record > data = GenerateData();
    std::vector< RecordId > rid;

    for( const Record& r: data )
    {
        rid.push_back( hf.Insert( r ) );
    }

    std::vector< Record > newData;
    Scaner scaner( hf );
    for( auto v : scaner )
    {
        newData.push_back( hf.Get( v ) );
    }

    for( const RecordId r : rid )
    {
        EXPECT_TRUE( std::find( scaner.begin(), scaner.end(), r ) != scaner.end() );
    }

    std::random_device rd;
    std::mt19937 g( rd() );
    std::shuffle( rid.begin(), rid.end(), g );
    std::sort( data.begin(), data.end() );

    std::sort( newData.begin(), newData.end() );

    EXPECT_EQ( data, newData );
}

