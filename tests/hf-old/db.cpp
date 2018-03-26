#include "gtest/gtest.h"
#include "hf/db.h"
#include "disk/unixfile.h"
#include "util/temp_path.h"

TEST(Db, CreteHeapFile)
{
    const std::string path = GetTempPath();
    std::size_t frameNo = 13;

    Db db( path, frameNo );
    EXPECT_NO_THROW( db.CreteHeapFile() );

}
