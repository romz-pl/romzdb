#include "gtest/gtest.h"
#include "disk/blockid.h"


TEST(BlockId, Comparision)
{
    EXPECT_TRUE( BlockId( 1 ) < BlockId( 2 ) );

    EXPECT_TRUE( BlockId( 2 ) > BlockId( 1 ) );

    EXPECT_TRUE( BlockId( 2 ) == BlockId( 2 ) );

    EXPECT_TRUE( BlockId( 1 ) != BlockId( 2 ) );

    EXPECT_TRUE( BlockId( 2 ) <= BlockId( 2 ) );
    EXPECT_TRUE( BlockId( 2 ) <= BlockId( 3 ) );

    EXPECT_TRUE( BlockId( 2 ) >= BlockId( 2 ) );
    EXPECT_TRUE( BlockId( 3 ) >= BlockId( 2 ) );
}

