#include <gtest/gtest.h>
#include <buffermgr.h>
#include <unixfile.h>
#include <cstdlib>
#include <dir.h>
#include <algorithm>
#include <random>


TEST(Dir, Constructor)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    PageId headerPage = pair.first ;
    DirPage dp( *pair.second, headerPage );
    dp.SetNextPage( InvalidPageId );
    bufferMgr.UnpinPage( headerPage );


    Dir dir( bufferMgr, headerPage );

}

TEST(Dir, Insert)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    PageId headerPage = pair.first ;
    DirPage dp( *pair.second, headerPage );
    dp.SetNextPage( InvalidPageId );
    bufferMgr.UnpinPage( headerPage );


    Dir dir( bufferMgr, headerPage );
    const std::size_t loopSize = 12;
    std::vector< PageId > pageId( loopSize );

    for( size_t i = 0; i < loopSize; i++ )
    {
        const PageId id = dir.Insert();
        pageId[ i ] = id;
    }


    // Delete pages in random order
    std::random_device rd;
    std::mt19937 g( rd() );
    std::shuffle( pageId.begin(), pageId.end(), g );

    for( PageId id : pageId )
    {
        EXPECT_NO_THROW( dir.Delete( id ) );
        EXPECT_FALSE( dir.Is( id) );
    }


}
