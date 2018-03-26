#include <random>
#include "gtest/gtest.h"
#include "hf/heapfile.h"
#include "disk/unixfile.h"
#include "hf/db.h"
#include "util/temp_path.h"


void Insert( HeapFile& hf, std::vector< RecordId >& allId )
{
    std::size_t loopSize = 128;
    for( std::size_t i = 0; i < loopSize; i++ )
    {
        std::cout << i << "\n" << std::flush;
        const std::string txt = Record::RandomString();
        const Record recA( txt );
        const RecordId rid = hf.Insert( recA );
        allId.push_back( rid  );
        const Record recB = hf.Get( rid );
        EXPECT_EQ( recA, recB );
    }

    EXPECT_EQ( hf.GetRecordNo(), loopSize );

    std::vector< RecordId > rids;
    hf.GetRid( rids );
    EXPECT_EQ( rids.size(), loopSize );
}

void Delete( HeapFile& hf, std::vector< RecordId >& allId )
{
    std::random_device rd;
    std::mt19937 g( rd() );

    std::shuffle( allId.begin(), allId.end(), g );

    for( RecordId v : allId )
    {
        EXPECT_NO_THROW( hf.Delete( v ) );
        EXPECT_ANY_THROW( hf.Get( v ) );
    }
    EXPECT_EQ( hf.GetRecordNo(), 0 );

    std::vector< RecordId > rids;
    hf.GetRid( rids );
    EXPECT_EQ( rids.size(), 0 );
}


TEST(HeapFile, GetInsertDelete)
{
    const std::string path = GetTempPath();
    // std::cout << path << "\n";
    std::size_t frameNo = 50;

    Db db( path, frameNo );
    HeapFile hf = db.CreteHeapFile();
    std::vector< RecordId > allId;

    Insert( hf, allId );
    Delete( hf, allId );

    allId.clear();

    Insert( hf, allId );
    Delete( hf, allId );
}
