#include <gtest/gtest.h>
#include <buffermgr.h>
#include <unixfile.h>
#include <cstdlib>


TEST(BufferMgr, GetPage)
{
    PageId pageId = 0;
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    EXPECT_ANY_THROW( bufferMgr.GetPage( pageId, false ) );

    auto pair = bufferMgr.GetNewPage();
    pageId = pair.first;
    EXPECT_ANY_THROW( bufferMgr.GetPage( pageId, false ) );
    EXPECT_NO_THROW( bufferMgr.GetPage( pageId, true ) );

    EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );
    EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.UnpinPage( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.UnpinPage( pageId + 1 ) );

    // bufferMgr.Print();
}

TEST(BufferMgr, TooSmallBuffer)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 3;
    BufferMgr bufferMgr( ds, numPages );
    std::vector< PageId > pageId;

    for( std::size_t i = 0; i < numPages; i++ )
    {
        auto pair = bufferMgr.GetNewPage( );
        pageId.push_back( pair.first );
    }

    EXPECT_ANY_THROW( bufferMgr.GetPage( numPages + 1, false ) );

    for( auto id : pageId )
        EXPECT_NO_THROW( bufferMgr.UnpinPage( id ) );

    // bufferMgr.Print();
}

TEST(BufferMgr, WritePage)
{
    PageId pageId = 1;
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 3;
    BufferMgr bufferMgr( ds, numPages );

    EXPECT_ANY_THROW( bufferMgr.MarkDirty( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.WritePage( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.UnpinPage( pageId ) );


    auto pair = bufferMgr.GetNewPage( );
    pageId = pair.first;
    EXPECT_NO_THROW( bufferMgr.MarkDirty( pageId ) );
    EXPECT_NO_THROW( bufferMgr.WritePage( pageId ) );
    EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );

    // bufferMgr.Print();
}

TEST(BufferMgr, UnpinPage)
{

    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 3;
    const std::size_t loopSize = 11;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    const PageId pageId = pair.first;

    for( std::size_t i = 0; i < loopSize; i++ )
        EXPECT_NO_THROW( bufferMgr.GetPage( pageId, true ) );

    for( std::size_t i = 0; i < loopSize + 1; i++ )
        EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );

    EXPECT_ANY_THROW( bufferMgr.UnpinPage( pageId ) );
}

TEST(BufferMgr, FlushPages)
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 3;
    const std::size_t loopSize = 11;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    const PageId pageId = pair.first;

    for( std::size_t i = 0; i < loopSize; i++ )
        EXPECT_NO_THROW( bufferMgr.GetPage( pageId, true ) );

    EXPECT_ANY_THROW( bufferMgr.FlushPages( ) );

    for( std::size_t i = 0; i < loopSize + 1; i++ )
        EXPECT_NO_THROW( bufferMgr.UnpinPage( pageId ) );

    EXPECT_NO_THROW( bufferMgr.FlushPages( ) );
}

