#include <gtest/gtest.h>
#include <cstdlib>
#include <heappage.h>
#include <record.h>


TEST(HeapPage, Get)
{
    Page page;
    HeapPage hp( page );
    const SlotId slotId = 0;
    // The page is empty
    EXPECT_ANY_THROW( hp.Get( slotId ) );
}

TEST(HeapPage, Insert)
{
    Record rec( {'a', 'b', 'c'} );
    Page page;
    HeapPage hp( page );

    SlotId slotId = hp.Insert( rec );
    // The page is NOT empty
    EXPECT_NO_THROW( hp.Get( slotId ) );
}
