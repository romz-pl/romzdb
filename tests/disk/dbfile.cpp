#include <gtest/gtest.h>
#include <dbfile.h>
#include <cstdlib>
#include <cstring>


TEST(DbFile, ReadWrite)
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

TEST(DbFile, OpenCreate)
{
    const uint32_t max_size = ( 1U << 14 );
    const std::string path = UnixFile::GetTempPath();

    EXPECT_NO_THROW( DbFile( path, max_size ) );
    EXPECT_NO_THROW( DbFile{ path } );

    EXPECT_ANY_THROW( DbFile( path, max_size ) );
}

TEST(DbFile, ToSmall)
{
    const uint32_t max_size = ( 1U << 10 );
    const std::string path = UnixFile::GetTempPath();

    EXPECT_ANY_THROW( DbFile( path, max_size ) );
}

TEST(DbFile, CreateBig)
{
    const uint32_t max_size = ( 1U << 24 );
    const std::string path = UnixFile::GetTempPath();

    EXPECT_NO_THROW( DbFile( path, max_size ) );

    DbFile db{ path };
    DiskBlock block;

    while( db.free_block_no() )
    {
        BlockId blockId = db.Alloc();
        EXPECT_NO_THROW( db.Read( blockId ) );
        EXPECT_NO_THROW( db.Write( block, blockId ) );
    }
    EXPECT_ANY_THROW( db.Alloc() );
}

TEST(DbFile, NotValidBlock)
{
    const std::string path = UnixFile::GetTempPath();
    const uint32_t max_size = ( 1U << 20 );
    DbFile db( path, max_size );

    const BlockId notValid( 0 );
    EXPECT_ANY_THROW( db.Read( notValid ) );

    DiskBlock block;
    EXPECT_ANY_THROW( db.Write( block, notValid ) );

    EXPECT_ANY_THROW( db.Dealloc( notValid ) );
}

