#include <random>
#include <algorithm>
#include "random_string.h"

//
//
//
std::string random_string( std::size_t max_length )
{
    const std::string alphabet( "0123456789"
                                "abcdefghijklmnopqrstuvwxyz"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

    std::random_device rd;
    std::mt19937 g( rd() );
    std::uniform_int_distribution< std::size_t > pick( 0, alphabet.size() );


    std::uniform_int_distribution< std::size_t > pick_length( 1, max_length + 1 );
    std::string::size_type length = pick_length( g );
    std::string s;
    s.reserve( length );

    while( length-- )
    {
        const char c = alphabet[ pick( g ) ];
        s += c;
    }
    return s;
}
