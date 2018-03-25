#include "gtest/gtest.h"
#include "buffer/buffclock.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"

TEST(BuffClock, Create)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    EXPECT_NO_THROW( BuffClock ( space, numPages ) );
}

TEST(BuffClock, Read)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );

    PageId ida( 0, 0 );
    EXPECT_NO_THROW( buff.alloc( ida ) );
    EXPECT_NO_THROW( buff.read ( ida ) );

    PageId idb( 0, 0 );
    EXPECT_NO_THROW( buff.alloc( idb ) );
    EXPECT_NO_THROW( buff.read ( idb ) );

    PageId idc( 0, 0 );
    EXPECT_NO_THROW( buff.alloc( idc ) );
    EXPECT_NO_THROW( buff.read ( idc ) );

    EXPECT_NO_THROW( buff.unpin( ida, true ) );
    EXPECT_NO_THROW( buff.unpin( ida, false ) );

    EXPECT_NO_THROW( buff.unpin( idb, true ) );
    EXPECT_NO_THROW( buff.unpin( idb, false ) );

    EXPECT_NO_THROW( buff.unpin( idc, true ) );
    EXPECT_NO_THROW( buff.unpin( idc, false ) );
}

TEST(BuffClock, ReadFaild)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );

    EXPECT_ANY_THROW( buff.read( PageId( 0, 0 ) ) );
    EXPECT_ANY_THROW( buff.read( PageId( 1, 0 ) ) );
    EXPECT_ANY_THROW( buff.read( PageId( 2, 0 ) ) );

    EXPECT_ANY_THROW( buff.read( PageId( 0, 1 ) ) );
    EXPECT_ANY_THROW( buff.read( PageId( 1, 1 ) ) );
    EXPECT_ANY_THROW( buff.read( PageId( 2, 1 ) ) );
}


/*
TEST(BuffClock, GetPage)
{
    PageId pageId( 1, 0 );
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( space, numPages );

    EXPECT_ANY_THROW( bufferMgr.Get( pageId, false ) );

    pageId = bufferMgr.GetNew();
    EXPECT_NO_THROW( bufferMgr.Get( pageId, false ) );
    EXPECT_NO_THROW( bufferMgr.Get( pageId, true ) );

    EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );
    EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.Unpin( pageId ) );

}

TEST(BufferMgr, TooSmallBuffer)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 3;
    BufferMgr bufferMgr( space, numPages );
    std::vector< PageId > pageId;

    for( std::size_t i = 0; i < numPages; i++ )
    {
        PageId v = bufferMgr.GetNew( );
        EXPECT_NO_THROW( bufferMgr.Get( v, true ) );
        pageId.push_back( v );
    }

    PageId pageIdEx( numPages + 1, 0 );
    EXPECT_ANY_THROW( bufferMgr.Get( pageIdEx, false ) );

    for( auto id : pageId )
        EXPECT_NO_THROW( bufferMgr.Unpin( id ) );

}

TEST(BufferMgr, MarkDirty)
{
    PageId pageId( 1, 0 );
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 3;
    BufferMgr bufferMgr( space, numPages );

    EXPECT_ANY_THROW( bufferMgr.MarkDirty( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.Unpin( pageId ) );


    pageId = bufferMgr.GetNew( );
    EXPECT_NO_THROW( bufferMgr.Get( pageId, true ) );
    EXPECT_NO_THROW( bufferMgr.MarkDirty( pageId ) );
    EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );
}

TEST(BufferMgr, Unpin)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 3;
    const std::size_t loopSize = 11;
    BufferMgr bufferMgr( space, numPages );

    const PageId pageId = bufferMgr.GetNew();

    for( std::size_t i = 0; i < loopSize; i++ )
        EXPECT_NO_THROW( bufferMgr.Get( pageId, true ) );

    for( std::size_t i = 0; i < loopSize; i++ )
        EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );

    EXPECT_ANY_THROW( bufferMgr.Unpin( pageId ) );
}
*/

