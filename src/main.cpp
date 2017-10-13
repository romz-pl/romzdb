#include <iostream>

#include "buffermgr.h"
#include "heappage.h"
#include "record.h"
#include "dirpage.h"
#include "dir.h"
#include <cassert>
#include <iostream>

void Test()
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t numPages = 100;
    BufferMgr bufferMgr( ds, numPages );

    auto pair = bufferMgr.GetNewPage();
    PageId headerPage = pair.first ;
    DirPage dp( *pair.second, headerPage );
    dp.SetNextPage( InvalidPageId );
    bufferMgr.UnpinPage( headerPage );


    Dir dir( bufferMgr, headerPage );
    const std::size_t loopSize = 5000;
    std::vector< PageId > pageId( loopSize );

    const std::size_t recLength = 3000;

    for( size_t i = 0; i < loopSize; i++ )
    {
        const PageId id = dir.Insert( recLength );
        pageId[ i ] = id;
        std::cout << id << " ";
    }

}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


