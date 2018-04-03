#include "gtest/gtest.h"
#include "hf/heap_file.h"
#include "util/temp_path.h"


TEST(HeapFile, create)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    EXPECT_NO_THROW( HeapFile{ bufferMgr } );
}


TEST(HeapFile, insert)
{
    const uint32_t max_size = ( 1U << 24 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf( bufferMgr );

    const std::uint32_t count = 3000;
    const int record_no = 300;
    for( int i = 0; i < record_no; i++ )
    {
        EXPECT_NO_THROW( hf.insert( count ) );
    }
}
