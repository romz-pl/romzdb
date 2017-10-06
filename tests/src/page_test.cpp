#include <gtest/gtest.h>
#include <page.h>
#include <cstdlib>
#include <cassert>

TEST(Page, Size)
{
    EXPECT_EQ( sizeof( Page ), Page::GetSize() );
}
