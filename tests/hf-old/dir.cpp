#include "gtest/gtest.h"
#include "hf/dir.h"
#include "disk/unixfile.h"


TEST(Dir, InsertDeleteGet)
{
    /*
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );


    Dir dir( bufferMgr, pageId );

    const Record rec( "ABC" );
    EXPECT_NO_THROW( dir.Insert( rec ) );
    EXPECT_NO_THROW( dir.Insert( rec ) );

    const auto rida = dir.Insert( Record( "a" ) );
    EXPECT_NO_THROW( dir.Get( rida ) );

    const auto ridb = dir.Insert( Record( "b" ) );
    EXPECT_NO_THROW( dir.Get( ridb ) );

    EXPECT_NO_THROW( dir.Delete( rida ) );
    EXPECT_ANY_THROW( dir.Get( rida ) );

    EXPECT_NO_THROW( dir.Delete( ridb ) );
    EXPECT_ANY_THROW( dir.Get( ridb ) );

    EXPECT_ANY_THROW( dir.Delete( rida ) );
    EXPECT_ANY_THROW( dir.Delete( ridb ) );

    */

}
