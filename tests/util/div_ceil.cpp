#include "gtest/gtest.h"
#include "util/div_ceil.h"


TEST(div_ceil, All)
{

    EXPECT_TRUE( div_ceil( 2, 2 ) == 1 );
    EXPECT_TRUE( div_ceil( 3, 2 ) == 2 );
    EXPECT_TRUE( div_ceil( 4, 2 ) == 2 );
    EXPECT_TRUE( div_ceil( 5, 2 ) == 3 );
    EXPECT_TRUE( div_ceil( 6, 2 ) == 3 );

    EXPECT_TRUE( div_ceil( 3, 3 ) == 1 );
    EXPECT_TRUE( div_ceil( 4, 3 ) == 2 );
    EXPECT_TRUE( div_ceil( 5, 3 ) == 2 );
    EXPECT_TRUE( div_ceil( 6, 3 ) == 2 );
    EXPECT_TRUE( div_ceil( 7, 3 ) == 3 );
}

