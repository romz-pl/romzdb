#include "gtest/gtest.h"
#include "hf/recordid.h"

TEST(RecordId, Copy)
{

    RecordId ra( PageId( 0, 0 ), SlotId( 0 ) );
    RecordId rb = ra;

    EXPECT_TRUE( ra == rb );

    RecordId rc( PageId( 3, 3 ), SlotId( 3 ) );
    rc = ra;

    EXPECT_TRUE( ra == rc );
}


TEST(RecordId, Comparision)
{
    EXPECT_TRUE( RecordId( PageId( 0, 0 ), SlotId( 0 ) ) < RecordId( PageId( 1, 0 ), SlotId( 0 ) ) );
    EXPECT_TRUE( RecordId( PageId( 0, 0 ), SlotId( 0 ) ) < RecordId( PageId( 0, 1 ), SlotId( 0 ) ) );
    EXPECT_TRUE( RecordId( PageId( 0, 0 ), SlotId( 0 ) ) < RecordId( PageId( 0, 0 ), SlotId( 1 ) ) );

    EXPECT_TRUE( RecordId( PageId( 2, 0 ), SlotId( 0 ) ) > RecordId( PageId( 1, 1 ), SlotId( 1 ) ) );

    EXPECT_TRUE( RecordId( PageId( 2, 2 ), SlotId( 0 ) ) == RecordId( PageId( 2, 2 ), SlotId( 0 ) ) );

    EXPECT_TRUE( RecordId( PageId( 3, 2 ), SlotId( 0 ) ) != RecordId( PageId( 2, 2 ), SlotId( 0 ) ) );

    EXPECT_TRUE( RecordId( PageId( 2, 2 ), SlotId( 0 ) ) <= RecordId( PageId( 2, 2 ), SlotId( 0 ) ) );
    EXPECT_TRUE( RecordId( PageId( 2, 2 ), SlotId( 0 ) ) <= RecordId( PageId( 2, 3 ), SlotId( 0 ) ) );

    EXPECT_TRUE( RecordId( PageId( 2, 2 ), SlotId( 3 ) ) >= RecordId( PageId( 2, 2 ), SlotId( 0 ) ) );
    EXPECT_TRUE( RecordId( PageId( 2, 2 ), SlotId( 0 ) ) == RecordId( PageId( 2, 2 ), SlotId( 0 ) ) );

}

TEST(RecordId, get)
{
    const PageId page_id( 1, 2 );
    const SlotId slot_id( 3 );
    RecordId rid( page_id, slot_id );


    EXPECT_TRUE( rid.get_page_id() == page_id );
    EXPECT_TRUE( rid.get_slot_id() == slot_id );
}
