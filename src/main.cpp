#include <iostream>
#include "heapfile.h"
#include "dir.h"
#include "db.h"
#include <cassert>
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
    std::size_t loopSize = 300000;
    for( std::size_t i = 0; i < loopSize; i++ )
    {
        const std::string txt = RandomString();
        const Record recA( txt );
        const RecordId rid = hf.Insert( recA );
        allId.push_back( rid  );
        const Record recB = hf.Get( rid );
        assert( recA == recB );
    }

    assert( hf.GetRecordNo() == loopSize );
}

void Delete( HeapFile& hf, std::vector< RecordId >& allId )
{
    std::random_device rd;
    std::mt19937 g( rd() );

    std::shuffle( allId.begin(), allId.end(), g );

    int i = 0;
    for( RecordId v : allId )
    // for( std::size_t i = 0; i < allId.size(); i++ )
    {
        i++;
        // RecordId v( allId[ i ] );
        hf.Delete( v );
        // hf.Get( v );
    }
    std::cout << i << "\n";
    std::size_t w = hf.GetRecordNo();

    std::cout << w <<"\n\n";
}


void Test()
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

    //Insert( hf, allId );
    //Delete( hf, allId );


}




int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


