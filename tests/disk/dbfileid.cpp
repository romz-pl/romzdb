#include "gtest/gtest.h"
#include "disk/dbfileid.h"


TEST(DbFileId, Comparision)
{
    EXPECT_TRUE( DbFileId( 1 ) < DbFileId( 2 ) );

    EXPECT_TRUE( DbFileId( 2 ) > DbFileId( 1 ) );

    EXPECT_TRUE( DbFileId( 2 ) == DbFileId( 2 ) );

    EXPECT_TRUE( DbFileId( 1 ) != DbFileId( 2 ) );

    EXPECT_TRUE( DbFileId( 2 ) <= DbFileId( 2 ) );
    EXPECT_TRUE( DbFileId( 2 ) <= DbFileId( 3 ) );

    EXPECT_TRUE( DbFileId( 2 ) >= DbFileId( 2 ) );
    EXPECT_TRUE( DbFileId( 3 ) >= DbFileId( 2 ) );
}

