#include <gtest/gtest.h>
#include <buffermgr.h>
#include <unixfile.h>
#include <cstdlib>


TEST(BufferMgr, GetPage)
{
    Page page( "abcd" );
    const PageId pageId = 0;
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    EXPECT_NO_THROW( ds.Write( page, pageId ) );

    EXPECT_NO_THROW( bufferMgr.GetPage( pageId, false ) );
    EXPECT_ANY_THROW( bufferMgr.GetPage( pageId, false ) );
    EXPECT_NO_THROW( bufferMgr.GetPage( pageId, true ) );

    EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );
    EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.UnpinPage( pageId + 1 ) );
}
