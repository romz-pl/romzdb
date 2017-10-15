#include <gtest/gtest.h>
#include <dirpage.h>
#include <unixfile.h>



TEST(DirPage, InsertDeleteGet)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );


    DirPage dp( bufferMgr, pageId );

    const Record rec( "ABC" );
    EXPECT_FALSE( dp.Insert( rec ).first );

    pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );
    dp.InsertPage( pageId );

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
