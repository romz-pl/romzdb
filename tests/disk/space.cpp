#include <gtest/gtest.h>
#include <space.h>
#include <unixfile.h>
#include <cstdlib>
#include <cstring>


TEST(DiskSpaceMgr, ReadWrite)
{
    const PageId pageId( 0 );
    Space space( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    DiskBlock block;
    char data[] = "abc";
    std::memcpy( block.GetData(), data, std::strlen( data ) );

    EXPECT_NO_THROW( space.Write( block, pageId ) );

    EXPECT_NO_THROW( space.Read( pageId ) );

    EXPECT_EQ( std::memcmp( space.Read( pageId ).GetData(), block.GetData(), DiskBlock::Size ), 0 );

    EXPECT_ANY_THROW( space.Read( PageId( 1 ) ) );
    EXPECT_ANY_THROW( space.Read( PageId( 2 ) ) );

    EXPECT_NO_THROW( space.Allocate() );
    EXPECT_NO_THROW( space.Allocate() );

    EXPECT_NO_THROW( space.Read( PageId( 1 ) ) );
    EXPECT_NO_THROW( space.Read( PageId( 2 ) ) );
}

