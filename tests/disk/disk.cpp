#include <gtest/gtest.h>
#include <disk.h>
#include <cstdlib>
#include <cstring>


TEST(Disk, ReadWrite)
{
    Disk disk( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    DiskBlock block;
    char data[] = "abc";
    std::memcpy( block.GetData(), data, std::strlen( data ) );

    EXPECT_NO_THROW( disk.Write( block, PageId( 0 ) ) );
    EXPECT_NO_THROW( disk.Read( PageId( 0 ) ) );
    EXPECT_EQ( std::memcmp( disk.Read( PageId( 0 ) ).GetData(), block.GetData(), DiskBlock::Size ), 0 );

    EXPECT_ANY_THROW( disk.Read( PageId( 1 ) ) );
    EXPECT_ANY_THROW( disk.Read( PageId( 2 ) ) );

    EXPECT_NO_THROW( disk.Allocate() );
    EXPECT_NO_THROW( disk.Allocate() );

    EXPECT_NO_THROW( disk.Read( PageId( 1 ) ) );
    EXPECT_NO_THROW( disk.Read( PageId( 2 ) ) );
}
