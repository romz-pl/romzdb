#include <algorithm>
#include <random>
#include "gtest/gtest.h"
#include "hf/heappage.h"
#include "hf/record.h"
#include "util/temp_path.h"

TEST(HeapPage, Constructor)
{
    const uint32_t max_size = ( 1U << 20 );
    Space space( GetTempPath(), max_size );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    PageId pageId( 1, 0 );
    EXPECT_ANY_THROW( HeapPage( bufferMgr, pageId ) );
}

TEST(HeapPage, Get)
{
    const uint32_t max_size = ( 1U << 20 );
    Space space( GetTempPath(), max_size );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    const PageId pageId = bufferMgr.GetNew();
    HeapPage hp( bufferMgr, pageId );
    const SlotId slotId( 0 );
    // The page is empty
    EXPECT_ANY_THROW( hp.Get( slotId ) );

}

TEST(HeapPage, Insert)
{
    const uint32_t max_size = ( 1U << 20 );
    Space space( GetTempPath(), max_size );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    const PageId pageId = bufferMgr.GetNew();
    HeapPage hp( bufferMgr, pageId );

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

TEST(HeapPage, Delete)
{
    const uint32_t max_size = ( 1U << 20 );
    Space space( GetTempPath(), max_size );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    const PageId pageId = bufferMgr.GetNew();
    HeapPage hp( bufferMgr, pageId );

    std::string str( "abc" );
    Record rec( str );
    const std::size_t recordNo = 123;

    for( std::size_t i = 0; i < recordNo; i++ )
    {
        EXPECT_NO_THROW( hp.Insert( rec ) );
    }

    EXPECT_EQ( hp.GetRecordNo(), recordNo );

    for( std::size_t i = 0; i < recordNo; i++ )
    {
        SlotId slotId( i );

        EXPECT_NO_THROW( hp.Delete( slotId ) );
        EXPECT_EQ( hp.GetRecordNo(), recordNo - 1 - i );
    }
    EXPECT_EQ( hp.GetRecordNo(), 0 );
}

TEST(HeapPage, CheckDelete)
{
    const uint32_t max_size = ( 1U << 20 );
    Space space( GetTempPath(), max_size );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    const PageId pageId = bufferMgr.GetNew();
    HeapPage hp( bufferMgr, pageId );

    SlotId ida = hp.Insert( Record( "A" ) );
    SlotId idb = hp.Insert( Record( "B" ) );

    EXPECT_NO_THROW( hp.Delete( ida ) );
    EXPECT_NO_THROW( hp.Delete( idb ) );
}
