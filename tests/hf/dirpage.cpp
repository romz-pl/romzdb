#include "gtest/gtest.h"
#include "hf/dirpage.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"


TEST(DirPage, InsertDeleteGet)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );


    PageId pageId = bufferMgr.alloc().first;
    bufferMgr.unpin( pageId, false );
    DirPage dp( bufferMgr, pageId );

    const Record rec( "ABC" );
    EXPECT_FALSE( dp.Insert( rec ).first );

    pageId = bufferMgr.alloc().first;
    bufferMgr.unpin( pageId, false );
    EXPECT_NO_THROW( dp.InsertHeapPage( pageId ) );

    EXPECT_TRUE( dp.Insert( rec ).first );

    const auto reca = dp.Insert( Record( "a" ) );
    EXPECT_TRUE( reca.first );
    EXPECT_TRUE( dp.Get( reca.second ).first );

    const auto recb = dp.Insert( Record( "b" ) );
    EXPECT_TRUE( recb.first );
    EXPECT_TRUE( dp.Get( recb.second ).first );

    EXPECT_TRUE( dp.Delete( reca.second ) );
    EXPECT_ANY_THROW( dp.Get( reca.second ).first );

    EXPECT_TRUE( dp.Delete( recb.second ) );
    EXPECT_ANY_THROW( dp.Get( recb.second ).first );

    EXPECT_ANY_THROW( dp.Delete( recb.second ) );
    EXPECT_ANY_THROW( dp.Delete( reca.second ) );

}
