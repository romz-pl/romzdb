#include "gtest/gtest.h"
#include "hf/record.h"

TEST(Record, Copy)
{
    Record rec_a( "aa" );
    Record rec_b( "bb" );

    EXPECT_TRUE( !(rec_a == rec_b) );

    rec_b = rec_a;
    EXPECT_TRUE( rec_a == rec_b );
}


TEST(Record, Comparision)
{
    Record rec_a( "aa" );
    Record rec_b( "bb" );

    EXPECT_TRUE( rec_a < rec_b );
}

