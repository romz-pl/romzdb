#include <gtest/gtest.h>
#include <dbfile.h>
#include <cstdlib>
#include <cstring>


TEST(DbFile, All)
{
    const std::string path = UnixFile::GetTempPath();
    const uint32_t max_size = ( 1U << 20 );
    DbFile db( path, max_size );

    EXPECT_ANY_THROW( db.Read( BlockId( 1 ) ) );

    DiskBlock block;
    EXPECT_ANY_THROW( db.Write( block, BlockId( 1 ) ) );

    BlockId blockId = db.Alloc();
    EXPECT_NO_THROW( db.Read( blockId ) );
    EXPECT_NO_THROW( db.Write( block, blockId ) );

    db.Dealloc( blockId );
    EXPECT_ANY_THROW( db.Read( blockId ) );
    EXPECT_ANY_THROW( db.Write( block, blockId ) );

}

