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

void Test()
{

    for( int i = 0; i < 1000; i++ )
    {
        std::string s = RandomString();
        std::cout << s.size() << " " << s << "\n";
    }
}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


