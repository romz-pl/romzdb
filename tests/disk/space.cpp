#include <gtest/gtest.h>
#include <space.h>
#include <unixfile.h>
#include <cstdlib>
#include <cstring>


TEST(Space, ReadWrite)
{
    const PageId pageId( 0 );
    Space space( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    const DiskBlock block ( std::string( "abc" ) );

    EXPECT_NO_THROW( space.Write( block, pageId ) );

    EXPECT_NO_THROW( space.Read( pageId ) );

    EXPECT_EQ( space.Read( pageId ), block );

    EXPECT_ANY_THROW( space.Read( PageId( 1 ) ) );
    EXPECT_ANY_THROW( space.Read( PageId( 2 ) ) );

    EXPECT_NO_THROW( space.Allocate() );
    EXPECT_NO_THROW( space.Allocate() );

    EXPECT_NO_THROW( space.Read( PageId( 1 ) ) );
    EXPECT_NO_THROW( space.Read( PageId( 2 ) ) );
}

