#include <gtest/gtest.h>
#include <diskspacemgr.h>
#include <unixfile.h>
#include <cstdlib>
#include <cstring>


TEST(DiskSpaceMgr, ReadWrite)
{
    const PageId pageId( 0 );
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );

    DiskBlock block;
    char data[] = "abc";
    std::memcpy( block.GetData(), data, std::strlen( data ) );

    EXPECT_NO_THROW( ds.Write( block, pageId ) );

    EXPECT_NO_THROW( ds.Read( pageId ) );

    EXPECT_EQ( std::memcmp( ds.Read( pageId ).GetData(), block.GetData(), DiskBlock::Size ), 0 );
}

