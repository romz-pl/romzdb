#include <gtest/gtest.h>
#include <dirpage.h>
#include <unixfile.h>
#include <cstdlib>
#include <algorithm>
#include <random>


TEST(DirPage, Insert)
{
    Page page;
    DirPage dp( page, 0 );

    const std::size_t recLength = 3;
    EXPECT_FALSE( dp.InsertRec( recLength ).first );
}

TEST(DirPage, Delete)
{
    Page page;
    DirPage dp( page, 0 );

    const PageId pageId = 1;
    const std::size_t recLength = 3;

    EXPECT_NO_THROW( dp.InsertRec( recLength ) );
    EXPECT_NO_THROW( dp.Delete( pageId, recLength ) );
}
