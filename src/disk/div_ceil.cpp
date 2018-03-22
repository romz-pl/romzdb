#include <cassert>
#include "div_ceil.h"

//
// Returns ceil( x / y )
//
std::uint32_t div_ceil( std::uint32_t x, std::uint32_t y )
{
    assert( y != 0 );
    return ( x + y - 1 ) / y;
}
