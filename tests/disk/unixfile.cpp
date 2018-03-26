#include "gtest/gtest.h"
#include "disk/unixfile.h"

//
// Return temporary path.
// The returned path can be safely used for creation of temporary file.
//
std::string GetTempPath()
{
    char dir[] = "/tmp/aaXXXXXXX";
    char *p = mkdtemp( dir );
    if( !p )
    {
        throw std::runtime_error( "Function 'mkdtemp failed" );
    }

    std::string path( dir );
    path += "/a.dat";
    return path;
}

TEST(UnixFile, Temporary)
{
    const std::string dir_a = "/tmp";
    EXPECT_NO_THROW( UnixFile{ dir_a } );

    const std::string dir_b = "./dev/full";
    EXPECT_ANY_THROW( UnixFile{ dir_b } );
}

TEST(UnixFile, OpenCreate)
{
    const std::string path = GetTempPath();

    // The file does not exist. It can not be opened
    EXPECT_ANY_THROW( UnixFile( path, UnixFile::Mode::Open ) );

    // The file does not exist. Hence, it can be created
    EXPECT_NO_THROW( UnixFile( path, UnixFile::Mode::Create ) );

    // The file exists. It can be opened.
    EXPECT_NO_THROW( UnixFile( path, UnixFile::Mode::Open ) );

    // The file exists. Hence, it can not be created
    EXPECT_ANY_THROW( UnixFile( path, UnixFile::Mode::Create ) );

    // The file can be opened more than one time.
    EXPECT_NO_THROW( UnixFile( path, UnixFile::Mode::Open ) );
}

std::vector< char > GetData()
{
    std::vector< char > ret = {'a', 'b', 'c' };
    return ret;
}

TEST(UnixFile, WriteFull)
{
    EXPECT_NO_THROW( UnixFile( "/dev/full", UnixFile::Mode::Open ) );
    UnixFile uf( "/dev/full", UnixFile::Mode::Open );

    const off_t offset = 0;
    std::uint32_t v = 0;
    EXPECT_ANY_THROW( uf.Write( &v, sizeof( v ), offset ) );
    EXPECT_ANY_THROW( uf.Fsync( ) );
}

TEST(UnixFile, ReadFull)
{
    EXPECT_NO_THROW( UnixFile( "/dev/null", UnixFile::Mode::Open ) );
    UnixFile uf( "/dev/null", UnixFile::Mode::Open );

    const off_t offset = 0;
    std::uint32_t v = 0;
    EXPECT_ANY_THROW( uf.Read( &v, sizeof( v ), offset ) );
    EXPECT_ANY_THROW( uf.Fsync( ) );
}

TEST(UnixFile, WriteRead)
{
    UnixFile uf( "/tmp" );

    const std::vector< char > data = GetData();
    
    const off_t offset = 0;
    EXPECT_NO_THROW( uf.Write( &( data[ 0 ] ), data.size(), offset ) );
    
    std::vector< char > tmp( data.size() );
    EXPECT_NO_THROW( uf.Read( &( tmp[ 0 ] ), tmp.size(), offset ) );
    
    EXPECT_TRUE( data == tmp );

    EXPECT_ANY_THROW( uf.Read( &( tmp[ 0 ] ), tmp.size(), offset + data.size() ) );
}

TEST(UnixFile, Read)
{
    UnixFile uf( "/tmp" );

    const std::size_t ss = 10;
    std::vector< char > tmp( ss );
    EXPECT_ANY_THROW( uf.Read( &( tmp[ 0 ] ), tmp.size(), 0 ) );
}

TEST(UnixFile, Fsync)
{
    UnixFile uf( "/tmp" );

    EXPECT_NO_THROW( uf.Fsync( ) );
}

