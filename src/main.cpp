#include <iostream>

#include "buffermgr.h"

void TestBufferMgr()
{
    Page page( "abcd" );
    const PageId pageId = 0;
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 10;
    BufferMgr bufferMgr( ds, numPages );

    bufferMgr.GetPage( pageId, false );
    bufferMgr.GetPage( pageId, true );
    bufferMgr.UnpinPage( pageId );
    bufferMgr.UnpinPage( pageId );
    // bufferMgr.UnpinPage( pageId + 1 );
}


int main()
{
    TestBufferMgr();
    std::cout << "OK\n";
    return 0;
}


