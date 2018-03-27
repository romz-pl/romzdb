#include "gtest/gtest.h"
#include "buffer/buffermgr.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"

TEST(BufferMgr, Create)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    EXPECT_NO_THROW( BufferMgr ( space, numPages ) );
}

TEST(BufferMgr, pin_unpin)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr buff( space, numPages );


    const PageId ida = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.pin ( ida ) );

    const PageId idb = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.pin ( idb ) );

    const PageId idc = buff.alloc( ).first;
    EXPECT_NO_THROW( buff.pin ( idc ) );

    EXPECT_NO_THROW( buff.unpin( ida, true ) );
    EXPECT_NO_THROW( buff.unpin( ida, false ) );

    EXPECT_NO_THROW( buff.unpin( idb, true ) );
    EXPECT_NO_THROW( buff.unpin( idb, false ) );

    EXPECT_NO_THROW( buff.unpin( idc, true ) );
    EXPECT_NO_THROW( buff.unpin( idc, false ) );
}

TEST(BufferMgr, ReadFaild)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr buff( space, numPages );

    EXPECT_ANY_THROW( buff.pin( PageId( 0, 0 ) ) );
    EXPECT_ANY_THROW( buff.pin( PageId( 1, 0 ) ) );
    EXPECT_ANY_THROW( buff.pin( PageId( 2, 0 ) ) );

    EXPECT_ANY_THROW( buff.pin( PageId( 0, 1 ) ) );
    EXPECT_ANY_THROW( buff.pin( PageId( 1, 1 ) ) );
    EXPECT_ANY_THROW( buff.pin( PageId( 2, 1 ) ) );
}

TEST(BufferMgr, Alloc)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr buff( space, numPages );
    std::vector< PageId > pages;

    for( std::size_t i = 0; i < 3 * numPages; i++ )
    {
        const PageId id = buff.alloc( ).first;
        pages.push_back( id );
        EXPECT_NO_THROW( buff.unpin( id, true ) );
    }

    for( std::size_t i = 0; i < pages.size(); i++ )
    {
        EXPECT_NO_THROW( buff.pin( pages[ i ] ) );
        EXPECT_NO_THROW( buff.unpin( pages[ i ], true ) );
    }
}

TEST(BufferMgr, Dispose)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr buff( space, numPages );

    std::vector< PageId > id_vec;


    for( std::size_t i = 0; i < numPages; i++ )
    {
        const PageId id = buff.alloc( ).first;
        id_vec.push_back( id );
    }

    for( auto id : id_vec )
    {
        EXPECT_NO_THROW( buff.unpin( id, false ) );
        EXPECT_NO_THROW( buff.dispose( id) );
    }
}

TEST(BufferMgr, Full)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr buff( space, numPages );
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

TEST(BufferMgr, Flush)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t numPages = 10;
    BufferMgr buff( space, numPages );

    const PageId id = buff.alloc( ).first;
    EXPECT_ANY_THROW( buff.flush( ) );

    EXPECT_NO_THROW( buff.unpin( id, false ) );
    EXPECT_NO_THROW( buff.flush( ) );

}
