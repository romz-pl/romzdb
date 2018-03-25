#include "gtest/gtest.h"
#include "buffer/buffermgr.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"


TEST(BufferMgr, GetPage)
{
    PageId pageId( 1, 0 );
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( space, numPages );

    EXPECT_ANY_THROW( bufferMgr.get( pageId ) );

    pageId = bufferMgr.alloc().first;
    EXPECT_NO_THROW( bufferMgr.get( pageId ) );
    EXPECT_NO_THROW( bufferMgr.get( pageId ) );

    EXPECT_NO_THROW( bufferMgr.unpin( pageId, true ) );
    EXPECT_NO_THROW( bufferMgr.unpin( pageId, true ) );
    EXPECT_NO_THROW( bufferMgr.unpin( pageId, true ) );

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
        PageId v = bufferMgr.alloc( ).first;
        EXPECT_NO_THROW( bufferMgr.get( v ) );
        pageId.push_back( v );
    }

    PageId pageIdEx( numPages + 1, 0 );
    EXPECT_ANY_THROW( bufferMgr.get( pageIdEx ) );

    for( auto id : pageId )
    {
        EXPECT_NO_THROW( bufferMgr.unpin( id, true ) );
        EXPECT_NO_THROW( bufferMgr.unpin( id, true ) );
    }

}

TEST(BufferMgr, MarkDirty)
{
    PageId pageId( 1, 0 );
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 3;
    BufferMgr bufferMgr( space, numPages );

    EXPECT_ANY_THROW( bufferMgr.unpin( pageId, true ) );

    pageId = bufferMgr.alloc( ).first;
    EXPECT_NO_THROW( bufferMgr.get( pageId ) );
    EXPECT_NO_THROW( bufferMgr.unpin( pageId, true ) );
    EXPECT_NO_THROW( bufferMgr.unpin( pageId, true ) );
}

TEST(BufferMgr, Unpin)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 3;
    const std::size_t loopSize = 11;
    BufferMgr bufferMgr( space, numPages );

    const PageId pageId = bufferMgr.alloc().first;

    for( std::size_t i = 0; i < loopSize; i++ )
        EXPECT_NO_THROW( bufferMgr.get( pageId ) );

    for( std::size_t i = 0; i < loopSize + 1; i++ )
        EXPECT_NO_THROW( bufferMgr.unpin( pageId, true ) );

    EXPECT_ANY_THROW( bufferMgr.unpin( pageId, true ) );
}


