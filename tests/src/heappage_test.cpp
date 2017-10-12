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
    Page page;
    HeapPage hp( page );

    for( int i = 0; i < 10; i++ )
    {
        std::string str( "abc:" );
        str += std::to_string( i );
        Record rec( str );
        SlotId slotId = hp.Insert( rec );
        // The page is NOT empty
        EXPECT_NO_THROW( hp.Get( slotId ) );
        Record ret = hp.Get( slotId );
        EXPECT_EQ( rec, ret );
    }
}
