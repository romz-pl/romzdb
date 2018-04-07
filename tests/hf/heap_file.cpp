#include <set>
#include "gtest/gtest.h"
#include "hf/heap_file.h"
#include "util/temp_path.h"
#include "util/random_string.h"


TEST(HeapFile, create)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    EXPECT_NO_THROW( HeapFile{ bufferMgr } );
}

TEST(HeapFile, open)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf_a{ bufferMgr };
    const PageId header_page = hf_a.get_header_page();

    EXPECT_NO_THROW( HeapFile( bufferMgr, header_page ) );
}


TEST(HeapFile, insert_into_dir)
{
    const uint32_t max_size = ( 1U << 24 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf( bufferMgr );

    const std::uint32_t count = 2000;
    const int record_no = 300;
    for( int i = 0; i < record_no; i++ )
    {
        EXPECT_NO_THROW( hf.insert_into_dir( count ) );
    }
}

TEST(HeapFile, insert_remove)
{
    const uint32_t max_size = ( 1U << 24 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf( bufferMgr );

    const std::uint32_t count = 2000;
    const int record_no = 300;
    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        EXPECT_NO_THROW( hf.insert( Record( str ) ) );
    }

    std::set< RecordId > sset;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( 200 );
        RecordId record_id = hf.insert( Record( str ) );
        sset.insert( record_id );
    }

    for( auto v : sset )
    {
        EXPECT_NO_THROW( hf.remove( v ) );
    }

    for( auto v : sset )
    {
        EXPECT_ANY_THROW( hf.remove( v ) );
    }
}

TEST(HeapFile, remove_from_dir)
{
    const uint32_t max_size = ( 1U << 24 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf( bufferMgr );

    std::multiset< PageId > mset;
    std::set< PageId > sset;

    const std::uint32_t count = 2000;
    const int record_no = 3000;
    for( int i = 0; i < record_no; i++ )
    {
        const PageId page_id  = hf.insert_into_dir( count );
        mset.insert( page_id );
        sset.insert( page_id );
    }

    for( auto v : sset )
    {
        EXPECT_ANY_THROW( hf.dispose_page( v ) );
    }

    for( auto v : mset )
    {
        EXPECT_NO_THROW( hf.remove_from_dir( v, count ) );
    }

    for( auto v : mset )
    {
        EXPECT_ANY_THROW( hf.remove_from_dir( v, count ) );
    }

    for( auto v : sset )
    {
        EXPECT_NO_THROW( hf.dispose_page( v ) );
        EXPECT_ANY_THROW( hf.dispose_page( v ) );
    }

    for( auto v : mset )
    {
        EXPECT_ANY_THROW( hf.remove_from_dir( v, count ) );
    }
}
