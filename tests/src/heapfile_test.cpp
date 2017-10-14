#include <gtest/gtest.h>
#include <heapfile.h>
#include <unixfile.h>



TEST(HeapFile, Insert)
{
    /*
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 1000;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    PageId headerPage = pair.first ;
    DirPage dp( *pair.second, headerPage );
    dp.SetNextPage( InvalidPageId );
    bufferMgr.UnpinPage( headerPage );

    HeapFile hf( bufferMgr, headerPage );

    Record rec( "0123456789ABC " );

    // std::size_t loopSize = 230;
    std::size_t loopSize = 220;
    for( std::size_t i = 0; i < loopSize; i++ )
    {
        // EXPECT_NO_THROW( hf.Insert( rec ) );
        hf.Insert( rec );
    }
    */

}
