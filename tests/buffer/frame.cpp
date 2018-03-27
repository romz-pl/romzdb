#include "gtest/gtest.h"
#include "buffer/frame.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"

TEST(Frame, invalid)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );

    Frame frame;

    EXPECT_ANY_THROW( frame.flush( space ) );
    EXPECT_ANY_THROW( frame.dispose( space ) );
    EXPECT_ANY_THROW( frame.unpin( false ) );
    EXPECT_ANY_THROW( frame.pin( ) );
    EXPECT_ANY_THROW( frame.write( space ) );
}
