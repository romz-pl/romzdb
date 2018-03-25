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

TEST(BuffClock, Dispose)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );

    PageId ida( 0, 0 );
    EXPECT_ANY_THROW( buff.dispose( ida ) );
    EXPECT_NO_THROW( buff.alloc( ida ) );
    EXPECT_NO_THROW( buff.read ( ida ) );
    EXPECT_NO_THROW( buff.unpin( ida, true ) );
    EXPECT_NO_THROW( buff.unpin( ida, false ) );
    EXPECT_NO_THROW( buff.dispose( ida ) );

    PageId idb( 0, 0 );
    EXPECT_ANY_THROW( buff.dispose( idb ) );
    EXPECT_NO_THROW( buff.alloc( idb ) );
    EXPECT_NO_THROW( buff.read ( idb ) );
    EXPECT_NO_THROW( buff.unpin( idb, true ) );
    EXPECT_NO_THROW( buff.unpin( idb, false ) );
    EXPECT_NO_THROW( buff.dispose( idb ) );
}

