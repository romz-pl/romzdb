#include <gtest/gtest.h>
#include <page.h>
#include <unixfile.h>
#include <cstdlib>
#include <cassert>


TEST(Page, Compare)
{
    Page pa( "abc" );
    Page pb( "xyz" );
    EXPECT_FALSE( pa == pb );
}

TEST(Page, IO)
{
    Page pa( "abc" );
    Page pb;
    EXPECT_FALSE( pa == pb );

    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    const PageId pageId = 0;
    EXPECT_NO_THROW( pa.Write( &uf, pageId ) );

    EXPECT_NO_THROW( pb.Read( &uf, pageId ) );
    EXPECT_TRUE( pa == pb );
}
