#include <gtest/gtest.h>
#include <buffermgr.h>
#include <unixfile.h>
#include <cstdlib>
#include <dir.h>

TEST(Dir, Constructor)
{
    /*
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    PageId headerPage = pair.first ;
    DirPage dp( bufferMgr, headerPage );
    dp.SetNextPage( InvalidPageId );
    bufferMgr.UnpinPage( headerPage );


    Dir dir( bufferMgr, headerPage );
*/
}

TEST(Dir, Insert)
{
    /*
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    PageId headerPage = pair.first ;
    DirPage dp( bufferMgr, headerPage );
    dp.SetNextPage( InvalidPageId );
    bufferMgr.UnpinPage( headerPage );


    Dir dir( bufferMgr, headerPage );
    const std::size_t loopSize = 50;

    const Record rec( std::vector< char >( 300, 'a' ) );

    for( size_t i = 0; i < loopSize; i++ )
    {
        const RecordId rid = dir.Insert( rec );
        EXPECT_NO_THROW( dir.Delete( rid ) );
    }
    */
}
