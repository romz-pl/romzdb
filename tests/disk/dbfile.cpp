#include <gtest/gtest.h>
#include <dbfile.h>
#include <cstdlib>
#include <cstring>


TEST(DbFile, All)
{
    const std::string path = UnixFile::GetTempPath();

    DbFile db( path, UnixFile::Mode::Create );

    EXPECT_ANY_THROW( db.Read( BlockId( 0 ) ) );

    DiskBlock block;
    EXPECT_ANY_THROW( db.Write( block, BlockId( 0 ) ) );

    BlockId blockId = db.Alloc();
    EXPECT_NO_THROW( db.Read( blockId ) );
    EXPECT_NO_THROW( db.Write( block, blockId ) );

    db.Dealloc( blockId );
    EXPECT_ANY_THROW( db.Read( blockId ) );
    EXPECT_ANY_THROW( db.Write( block, blockId ) );

}

