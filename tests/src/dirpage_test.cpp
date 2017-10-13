#include <gtest/gtest.h>
#include <dirpage.h>
#include <unixfile.h>
#include <cstdlib>
#include <algorithm>
#include <random>


TEST(DirPage, Insert)
{
    Page page;
    DirPage dp( page );

    const PageId pageId = 1;
    EXPECT_NO_THROW( dp.Insert( pageId ) );
    EXPECT_ANY_THROW( dp.Insert( pageId ) );
}

TEST(DirPage, InsertEx)
{
    Page page;
    DirPage dp( page );

    PageId pageId = 1;
    std::vector< PageId > vec;

    while( !dp.IsFull() )
    {
        vec.push_back( pageId );
        EXPECT_NO_THROW( dp.Insert( pageId ) );
        pageId++;
    }

    // Delete pages in random order
    std::random_device rd;
    std::mt19937 g( rd() );
    std::shuffle( vec.begin(), vec.end(), g );
    for( PageId v : vec )
    {
        EXPECT_NO_THROW( dp.Delete( v ) );
    }
}

TEST(DirPage, Delete)
{
    Page page;
    DirPage dp( page );

    const PageId pageId = 1;
    EXPECT_NO_THROW( dp.Insert( pageId ) );
    EXPECT_NO_THROW( dp.Delete( pageId ) );
}
