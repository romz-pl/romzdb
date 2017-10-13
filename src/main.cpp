#include <iostream>

#include "buffermgr.h"
#include "heappage.h"
#include "record.h"
#include "dirpage.h"
#include <cassert>

void Test()
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

    // bufferMgr.GetPage( numPages + 1, false );

    for( auto id : pageId )
        bufferMgr.UnpinPage( id );

}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


