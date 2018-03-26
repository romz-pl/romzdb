#include "gtest/gtest.h"
#include "disk/dbfile.h"


TEST(DbFile, ReadWrite)
{
    const uint32_t max_size = ( 1U << 20 );
    UnixFile uf( "/tmp" );
    DbFile db( uf, max_size );

    EXPECT_ANY_THROW( db.Read( BlockId( 1 ) ) );

    DiskBlock block;
    EXPECT_ANY_THROW( db.Write( block, BlockId( 1 ) ) );

    BlockId blockId = db.Alloc();
    EXPECT_NO_THROW( db.Read( blockId ) );
    EXPECT_NO_THROW( db.Write( block, blockId ) );

    EXPECT_NO_THROW( db.Dealloc( blockId ) );
    EXPECT_ANY_THROW( db.Read( blockId ) );
    EXPECT_ANY_THROW( db.Write( block, blockId ) );

    EXPECT_ANY_THROW( db.Dealloc( blockId ) );

}

TEST(DbFile, OpenCreate)
{
    const uint32_t max_size = ( 1U << 14 );

    UnixFile uf( "/tmp" );
    EXPECT_NO_THROW( DbFile( uf, max_size ) );
}

TEST(DbFile, ToSmall)
{
    const uint32_t max_size = ( 1U << 10 );
    UnixFile uf( "/tmp" );
    EXPECT_ANY_THROW( DbFile( uf, max_size ) );
}

TEST(DbFile, BigFile)
{
    const uint32_t max_size = ( 1U << 29 );
    UnixFile uf( "/tmp" );

    EXPECT_NO_THROW( DbFile( uf, max_size ) );
}

TEST(DbFile, BadAlloc)
{
    const uint32_t max_size = ( 1U << 15 );
    UnixFile uf( "/tmp" );
    DbFile db( uf, max_size );

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
    UnixFile uf( "/tmp" );
    const uint32_t max_size = ( 1U << 20 );
    DbFile db( uf, max_size );

    const BlockId notValid( 0 );
    EXPECT_ANY_THROW( db.Read( notValid ) );

    DiskBlock block;
    EXPECT_ANY_THROW( db.Write( block, notValid ) );

    EXPECT_ANY_THROW( db.Dealloc( notValid ) );
}

