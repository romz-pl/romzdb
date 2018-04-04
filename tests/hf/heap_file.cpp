#include <set>
#include "gtest/gtest.h"
#include "hf/heap_file.h"
#include "util/temp_path.h"


TEST(HeapFile, create)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    EXPECT_NO_THROW( HeapFile{ bufferMgr } );
}


TEST(HeapFile, insert)
{
    const uint32_t max_size = ( 1U << 24 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf( bufferMgr );

    const std::uint32_t count = 3000;
    const int record_no = 300;
    for( int i = 0; i < record_no; i++ )
    {
        EXPECT_NO_THROW( hf.insert( count ) );
    }
}

TEST(HeapFile, remove)
{
    const uint32_t max_size = ( 1U << 24 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf( bufferMgr );

    std::multiset< PageId > mset;
    std::set< PageId > sset;

    const std::uint32_t count = 3000;
    const int record_no = 3000;
    for( int i = 0; i < record_no; i++ )
    {
        const PageId page_id  = hf.insert( count );
        // std::cout << page_id.get_block_id().GetValue() << " " << std::flush;
        mset.insert( page_id );
        sset.insert( page_id );
    }

    for( auto v : mset )
    {
        EXPECT_NO_THROW( hf.remove( v, count ) );
    }

    for( auto v : sset )
    {
        // std::cout << v.get_block_id().GetValue() << " " << std::flush;
        EXPECT_NO_THROW( hf.free_page( v ) );
        EXPECT_ANY_THROW( hf.free_page( v ) );
    }

//    for( auto v : mset )
//    {
//        EXPECT_ANY_THROW( hf.remove( v, count ) );
//    }
}
