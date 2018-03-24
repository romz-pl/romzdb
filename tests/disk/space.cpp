#include "gtest/gtest.h"
#include "disk/space.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"

TEST(Space, OpenCreate)
{
    const uint32_t max_size = ( 1U << 20 );
    const std::string path = GetTempPath();

    {
        // Create file
        DbFile file( path, max_size );
        Space space{ file };
        EXPECT_NO_THROW( space.Alloc() );
    }

    {
        // Open file
        DbFile file( path );
        Space space{ file };
        EXPECT_NO_THROW( space.Alloc() );
    }
}

TEST(Space, Add)
{
    Space space;

    const uint32_t max_size = ( 1U << 20 );

    DbFile file_a( GetTempPath(), max_size );
    EXPECT_NO_THROW( space.Add( file_a ) );

    DbFile file_b( GetTempPath(), max_size );
    EXPECT_NO_THROW( space.Add( file_b ) );

    DbFile file_c( GetTempPath(), max_size );
    EXPECT_NO_THROW( space.Add( file_c ) );
}

TEST(Space, Remove)
{
    Space space;

    const uint32_t max_size = ( 1U << 20 );

    DbFile file_a( GetTempPath(), max_size );
    DbFileId id = space.Add( file_a );

    EXPECT_NO_THROW( space.Remove( id ) );
    EXPECT_ANY_THROW( space.Remove( id ) );
}

TEST(Space, Full)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );
    const DiskBlock block ( std::string( "abc" ) );

    while( !space.full() )
    {
        const PageId pageId = space.Alloc();
        EXPECT_NO_THROW( space.Write( block, pageId ) );
    }

    EXPECT_ANY_THROW( space.Alloc() );
}

TEST(Space, WriteIncorrect)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );

    const DiskBlock block ( std::string( "abc" ) );

    EXPECT_ANY_THROW( space.Write( block, PageId( 1, 0 ) ) );
    EXPECT_ANY_THROW( space.Write( block, PageId( 1, 1 ) ) );
}

TEST(Space, ReadIncorrect)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );

    EXPECT_ANY_THROW( space.Read( PageId( 1, 0 ) ) );
    EXPECT_ANY_THROW( space.Read( PageId( 1, 1 ) ) );
}

TEST(Space, ReadWrite)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );

    const DiskBlock block ( std::string( "abc" ) );

    const PageId pageId = space.Alloc();
    EXPECT_NO_THROW( space.Write( block, pageId ) );
    EXPECT_NO_THROW( space.Read( pageId ) );

    EXPECT_EQ( space.Read( pageId ), block );

    EXPECT_NO_THROW( space.Dealloc( pageId ) );

    EXPECT_ANY_THROW( space.Write( block, pageId ) );
    EXPECT_ANY_THROW( space.Read( pageId ) );
}

TEST(Space, Dealloc)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( GetTempPath(), max_size );
    Space space( db_file );

    EXPECT_ANY_THROW( space.Dealloc( PageId( 1, 0 ) ) );
    EXPECT_ANY_THROW( space.Dealloc( PageId( 1, 1 ) ) );

}

