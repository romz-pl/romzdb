#include <gtest/gtest.h>
#include <cstdlib>
#include <heappage.h>
#include <record.h>
#include <algorithm>
#include <random>

TEST(HeapPage, Get)
{
    /*
    Page page;
    HeapPage hp( page );
    const SlotId slotId = 0;
    // The page is empty
    EXPECT_ANY_THROW( hp.Get( slotId ) );
    */
}

TEST(HeapPage, Insert)
{
    /*
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
    */
}

TEST(HeapPage, Delete)
{
    /*
    Page page;
    HeapPage hp( page );
    std::string str( "abc" );
    Record rec( str );
    const std::size_t recordNo = 123;

    for( std::size_t i = 0; i < recordNo; i++ )
    {
        EXPECT_NO_THROW( hp.Insert( rec ) );
    }

    EXPECT_EQ( hp.GetRecordNo(), recordNo );

    // Delete records in the random order
    std::random_device rd;
    std::mt19937 rng( rd() );

    for( std::size_t i = 0; i < recordNo; i++ )
    {
        std::uniform_int_distribution< SlotId > uni( 0 , hp.GetRecordNo() - 1 );
        SlotId slotId = uni( rng );

        EXPECT_NO_THROW( hp.Delete( slotId ) );
        EXPECT_EQ( hp.GetRecordNo(), recordNo - 1 - i );
    }
    EXPECT_EQ( hp.GetRecordNo(), 0 );
    */

}
