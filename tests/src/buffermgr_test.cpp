#include <gtest/gtest.h>
#include <buffermgr.h>
#include <unixfile.h>
#include <cstdlib>


TEST(BufferMgr, GetPage)
{
    PageId pageId( 0 );
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    EXPECT_ANY_THROW( bufferMgr.Get( pageId, false ) );

    auto pair = bufferMgr.GetNew();
    pageId = pair.first;
    EXPECT_ANY_THROW( bufferMgr.Get( pageId, false ) );
    EXPECT_NO_THROW( bufferMgr.Get( pageId, true ) );

    EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );
    EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.Unpin( pageId ) );

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
        auto pair = bufferMgr.GetNew( );
        pageId.push_back( pair.first );
    }

    PageId pageIdEx( numPages + 1 );
    EXPECT_ANY_THROW( bufferMgr.Get( pageIdEx, false ) );

    for( auto id : pageId )
        EXPECT_NO_THROW( bufferMgr.Unpin( id ) );
}

TEST(BufferMgr, MarkDirty)
{
    PageId pageId( 1 );
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 3;
    BufferMgr bufferMgr( ds, numPages );

    EXPECT_ANY_THROW( bufferMgr.MarkDirty( pageId ) );
    EXPECT_ANY_THROW( bufferMgr.Unpin( pageId ) );


    auto pair = bufferMgr.GetNew( );
    pageId = pair.first;
    EXPECT_NO_THROW( bufferMgr.MarkDirty( pageId ) );
    EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );
}

TEST(BufferMgr, Unpin)
{

    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 3;
    const std::size_t loopSize = 11;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNew();
    const PageId pageId = pair.first;

    for( std::size_t i = 0; i < loopSize; i++ )
        EXPECT_NO_THROW( bufferMgr.Get( pageId, true ) );

    for( std::size_t i = 0; i < loopSize + 1; i++ )
        EXPECT_NO_THROW( bufferMgr.Unpin( pageId ) );

    EXPECT_ANY_THROW( bufferMgr.Unpin( pageId ) );
}


