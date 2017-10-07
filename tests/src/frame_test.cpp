#include <gtest/gtest.h>
#include <diskspacemgr.h>
#include <unixfile.h>
#include <cstdlib>


TEST(DiskSpaceMgr, IO)
{
    Page pa( "abc");
    const PageId pageId = 0;
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );

    EXPECT_NO_THROW( ds.Write( pa, pageId ) );

    EXPECT_NO_THROW( ds.Read( pageId ) );
    EXPECT_EQ( ds.Read( pageId ), pa );
}
