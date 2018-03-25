#include "gtest/gtest.h"
#include "hf/dirpage.h"
#include "util/temp_path.h"


TEST(DirPage, get_set_next_page)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    std::pair< PageId, DiskBlock* > ret = bufferMgr.alloc();
    DirPage dp( ret.second );

    const PageId ida( 3 , 4 );
    dp.set_next_page( ida );
    const PageId idb = dp.get_next_page();

    EXPECT_TRUE( ida == idb );
    bufferMgr.unpin( ret.first, true );
}


TEST(DirPage, add_remove)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    std::pair< PageId, DiskBlock* > ret = bufferMgr.alloc();
    DirPage dp( ret.second );

    const PageId ida( 3 , 4 );
    EXPECT_NO_THROW( dp.init() );
    EXPECT_TRUE( dp.add( ida, 10 ) );
    EXPECT_TRUE( dp.remove( ida ) );
    EXPECT_FALSE( dp.remove( ida ) );
    bufferMgr.unpin( ret.first, false );
}
