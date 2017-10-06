#include <gtest/gtest.h>
#include <unixfile.h>
#include <cstdlib>
#include <cassert>


TEST(UnixFile, Open)
{
    const std::string path = UnixFile::GetTempPath();

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

TEST(UnixFile, Write)
{
    const std::string path = UnixFile::GetTempPath();

    UnixFile uf( path, UnixFile::Mode::Create );

    const std::vector< char > data = GetData();
    
    const off_t offset = 0;
    EXPECT_NO_THROW( uf.Write( &( data[ 0 ] ), data.size(), offset ) );
    
    std::vector< char > tmp( data.size() );
    EXPECT_NO_THROW( uf.Read( &( tmp[ 0 ] ), tmp.size(), offset ) );
    
    EXPECT_TRUE( data == tmp );
    
    EXPECT_ANY_THROW( uf.Read( &( tmp[ 0 ] ), tmp.size(), offset + 1 ) );
}
