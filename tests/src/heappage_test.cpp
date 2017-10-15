#include <gtest/gtest.h>
#include <cstdlib>
#include <heappage.h>
#include <record.h>
#include <algorithm>
#include <random>

TEST(HeapPage, Constructor)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    PageId pageId( 0 );
    EXPECT_ANY_THROW( HeapPage( bufferMgr, pageId ) );
}

TEST(HeapPage, Get)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    const PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );

    HeapPage hp( bufferMgr, pageId );
    const SlotId slotId( 0 );
    // The page is empty
    EXPECT_ANY_THROW( hp.Get( slotId ) );

}

TEST(HeapPage, Insert)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    const PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );

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
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    const PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );

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
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    const PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );

    HeapPage hp( bufferMgr, pageId );

    SlotId ida = hp.Insert( Record( "A" ) );
    SlotId idb = hp.Insert( Record( "B" ) );

    EXPECT_NO_THROW( hp.Delete( ida ) );
    EXPECT_NO_THROW( hp.Delete( idb ) );
}
