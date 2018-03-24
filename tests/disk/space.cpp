#include "gtest/gtest.h"
#include "disk/space.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"

TEST(Space, OpenCreate)
{
    const uint32_t max_size = ( 1U << 20 );
    const std::string path = GetTempPath();
    EXPECT_NO_THROW( Space( path, max_size ) );
    EXPECT_ANY_THROW( Space( path, max_size ) );
    EXPECT_NO_THROW( Space{ path } );
}


TEST(Space, ReadWrite)
{

    const uint32_t max_size = ( 1U << 20 );
    Space space( GetTempPath(), max_size );

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

