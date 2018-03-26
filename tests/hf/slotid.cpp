#include "gtest/gtest.h"
#include "hf/slotid.h"

TEST(SlotId, Copy)
{
    SlotId sa( 1 );
    SlotId sb = sa;

    EXPECT_TRUE( sa == sb );

    SlotId sc( 3 );
    sc = sa;

    EXPECT_TRUE( sa == sc );
}


TEST(SlotId, Comparision)
{
    EXPECT_TRUE( SlotId( 1 ) < SlotId( 2 ) );

    EXPECT_TRUE( SlotId( 2 ) > SlotId( 1 ) );

    EXPECT_TRUE( SlotId( 2 ) == SlotId( 2 ) );

    EXPECT_TRUE( SlotId( 1 ) != SlotId( 2 ) );

    EXPECT_TRUE( SlotId( 2 ) <= SlotId( 2 ) );
    EXPECT_TRUE( SlotId( 2 ) <= SlotId( 3 ) );

    EXPECT_TRUE( SlotId( 2 ) >= SlotId( 2 ) );
    EXPECT_TRUE( SlotId( 3 ) >= SlotId( 2 ) );
}

