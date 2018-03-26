#include "gtest/gtest.h"
#include "hf/record.h"

TEST(Record, Copy)
{
    const PageId page_id( 1, 2 );
    const SlotId slot_id( 3 );
    Record rec( page_id, slot_id );


    EXPECT_TRUE( rec.get_page_id() == page_id );
    EXPECT_TRUE( rec.get_slot_id() == slot_id );

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

