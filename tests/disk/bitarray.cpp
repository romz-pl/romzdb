#include <gtest/gtest.h>
#include <bitarray.h>
#include <cstdlib>
#include <cstring>


TEST(BitArray, All)
{
    const std::uint32_t bits_no = 10000;
    BitArray< bits_no > ba;

    for( std::uint32_t k = 0; k < bits_no; k++ )
    {
        EXPECT_FALSE( ba.test( k ) );

        EXPECT_NO_THROW( ba.set( k ) );
        EXPECT_TRUE( ba.test( k ) );

        EXPECT_NO_THROW( ba.reset( k ) );
        EXPECT_FALSE( ba.test( k ) );

        EXPECT_NO_THROW( ba.flip( k ) );
        EXPECT_TRUE( ba.test( k ) );
    }

}

