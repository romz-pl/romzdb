#include <gtest/gtest.h>
#include <space.h>
#include <unixfile.h>
#include <cstdlib>
#include <cstring>


TEST(Space, ReadWrite)
{

    Space space( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    const DiskBlock block ( std::string( "abc" ) );

    EXPECT_ANY_THROW( space.Write( block, PageId( 1 ) ) );

    const PageId pageId = space.Alloc();
    EXPECT_NO_THROW( space.Write( block, pageId ) );
    EXPECT_NO_THROW( space.Read( pageId ) );

    EXPECT_EQ( space.Read( pageId ), block );

    EXPECT_NO_THROW( space.Dealloc( pageId ) );

    EXPECT_ANY_THROW( space.Write( block, pageId ) );
    EXPECT_ANY_THROW( space.Read( pageId ) );

}

