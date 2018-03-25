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


    const PageId ida = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.get ( ida ) );

    const PageId idb = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.get ( idb ) );

    const PageId idc = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.get ( idc ) );

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

    EXPECT_ANY_THROW( buff.get( PageId( 0, 0 ) ) );
    EXPECT_ANY_THROW( buff.get( PageId( 1, 0 ) ) );
    EXPECT_ANY_THROW( buff.get( PageId( 2, 0 ) ) );

    EXPECT_ANY_THROW( buff.get( PageId( 0, 1 ) ) );
    EXPECT_ANY_THROW( buff.get( PageId( 1, 1 ) ) );
    EXPECT_ANY_THROW( buff.get( PageId( 2, 1 ) ) );
}

TEST(BuffClock, Alloc)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );
    std::vector< PageId > pages;

    for( std::size_t i = 0; i < 3 * numPages; i++ )
    {
        const PageId id = buff.alloc( ).first;
        pages.push_back( id );
        EXPECT_NO_THROW( buff.unpin( id, true ) );
    }

    for( std::size_t i = 0; i < pages.size(); i++ )
    {
        EXPECT_NO_THROW( buff.get( pages[ i ] ) );
        EXPECT_NO_THROW( buff.unpin( pages[ i ], true ) );
    }
}

TEST(BuffClock, Dispose)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );


    EXPECT_ANY_THROW( buff.dispose( PageId( 0, 0 ) ) );
    const PageId ida = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.get ( ida ) );
    EXPECT_NO_THROW( buff.unpin( ida, true ) );
    EXPECT_NO_THROW( buff.unpin( ida, false ) );
    EXPECT_ANY_THROW( buff.unpin( ida, false ) );
    EXPECT_NO_THROW( buff.dispose( ida ) );

    EXPECT_ANY_THROW( buff.dispose( PageId( 1, 0 ) ) );
    const PageId idb = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.get ( idb ) );
    EXPECT_NO_THROW( buff.unpin( idb, true ) );
    EXPECT_NO_THROW( buff.unpin( idb, false ) );
    EXPECT_ANY_THROW( buff.unpin( idb, false ) );
    EXPECT_NO_THROW( buff.dispose( idb ) );

    EXPECT_ANY_THROW( buff.unpin( PageId( 10, 0 ), false ) );
    EXPECT_ANY_THROW( buff.unpin( PageId( 11, 0 ), false ) );
}

TEST(BuffClock, Full)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );
    std::vector< PageId > pages;

    for( std::uint32_t i = 0; i < numPages; i++ )
    {
        EXPECT_NO_THROW( pages.push_back( buff.alloc( ).first ) );
    }

    EXPECT_ANY_THROW( buff.alloc( ) );

    for( std::uint32_t i = 0; i < numPages; i++ )
    {
        EXPECT_NO_THROW( buff.unpin( pages[ i ], false ) );
    }
}

TEST(BuffClock, Flush)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BuffClock buff( space, numPages );

    const PageId id = buff.alloc( ).first;
    EXPECT_ANY_THROW( buff.flush( ) );

    EXPECT_NO_THROW( buff.unpin( id, false ) );
    EXPECT_NO_THROW( buff.flush( ) );

}
