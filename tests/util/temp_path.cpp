#include "gtest/gtest.h"
#include "util/temp_path.h"


TEST(temp_path, All)
{

    EXPECT_TRUE( !GetTempPath().empty() );

}

