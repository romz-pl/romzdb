#include <gtest/gtest.h>
#include <db.h>
#include <unixfile.h>

TEST(Db, CreteHeapFile)
{
    const std::string path = UnixFile::GetTempPath();
    std::size_t frameNo = 13;

    Db db( path, frameNo );
    EXPECT_NO_THROW( db.CreteHeapFile() );

}
