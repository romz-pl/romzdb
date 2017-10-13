#include <iostream>

#include "buffermgr.h"
#include "heappage.h"
#include "record.h"
#include "dirpage.h"
#include "dir.h"
#include <cassert>

void Test()
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


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


