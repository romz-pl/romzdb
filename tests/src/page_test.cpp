#include <gtest/gtest.h>
#include <page.h>
#include <unixfile.h>
#include <cstdlib>
#include <cassert>
#include <algorithm>


TEST(Page, Compare)
{
    Page pa( "abc" );
    Page pb( "xyz" );
    EXPECT_FALSE( pa == pb );
}

TEST(Page, IO)
{
    Page pa( "abc" );
    Page pb;
    EXPECT_FALSE( pa == pb );

    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    const PageId pageId = 0;
    EXPECT_NO_THROW( pa.Write( uf, pageId ) );

    EXPECT_NO_THROW( pb.Read( uf, pageId ) );
    EXPECT_TRUE( pa == pb );
}

TEST(Page, IOsequence)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );

    const PageId maxPageId = 200;
    std::vector< PageId > seq( 13 );

    std::generate( seq.begin(), seq.end(), []{ return rand() % maxPageId; });

    for( PageId id : seq )
    {
	// std::cout << id << " ";
        std::string txt( "abc-" + std::to_string( id ) );
        Page pa( txt );
        
        EXPECT_NO_THROW( pa.Write( uf, id ) );
        
        Page pb;
        EXPECT_NO_THROW( pb.Read( uf, id ) );
        EXPECT_TRUE( pa == pb );
    }
}

TEST(Page, GetSet)
{
    Page pa;
    int val = 123;
    PageOffset offset = 1;
    pa.Set( val, offset );
    EXPECT_EQ( val, pa.Get< int >( offset ) );

    offset = Page::PageSize;
    EXPECT_ANY_THROW( pa.Set( val, offset ) );
    EXPECT_ANY_THROW( pa.Get< int >( offset ) );

    offset = Page::PageSize - sizeof( int );
    EXPECT_NO_THROW( pa.Set( val, offset ) );
    EXPECT_NO_THROW( pa.Get< int >( offset ) );
    EXPECT_EQ( val, pa.Get< int >( offset ) );

    // std::cout << offset << " " << Page::PageSize << " " << sizeof( int );

}
 
