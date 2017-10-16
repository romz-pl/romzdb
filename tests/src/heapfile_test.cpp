#include <gtest/gtest.h>
#include <heapfile.h>
#include <unixfile.h>
#include <db.h>
#include <random>
#include <algorithm>

std::string RandomString( )
{
    const std::string alphabet( "0123456789"
                                "abcdefghijklmnopqrstuvwxyz"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

    std::random_device rd;
    std::mt19937 g( rd() );
    std::uniform_int_distribution< std::string::size_type > pick( 0, alphabet.size() );

    std::string::size_type length = pick( g );
    std::string s;
    s.reserve( length );

    while( length-- )
        s += alphabet[ pick( g ) ];

    return s;
}

void Insert( HeapFile& hf, std::vector< RecordId >& allId )
{
    std::size_t loopSize = 200000;
    for( std::size_t i = 0; i < loopSize; i++ )
    {
        const std::string txt = RandomString();
        const Record recA( txt );
        const RecordId rid = hf.Insert( recA );
        allId.push_back( rid  );
        const Record recB = hf.Get( rid );
        EXPECT_EQ( recA, recB );
    }
}

void Delete( HeapFile& hf, std::vector< RecordId >& allId )
{
    std::random_device rd;
    std::mt19937 g( rd() );

    std::shuffle( allId.begin(), allId.end(), g );

    for( RecordId v : allId )
    {
        EXPECT_NO_THROW( hf.Delete( v ) );
        EXPECT_ANY_THROW( hf.Get( v ) );
    }
}


TEST(HeapFile, GetInsertDelete)
{
    const std::string path = UnixFile::GetTempPath();
    // std::cout << path << "\n";
    std::size_t frameNo = 50;

    Db db( path, frameNo );
    HeapFile hf = db.CreteHeapFile();
    std::vector< RecordId > allId;

    Insert( hf, allId );
    Delete( hf, allId );

    allId.clear();

    Insert( hf, allId );
    Delete( hf, allId );


}
