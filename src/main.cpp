#include <iostream>

#include "buffermgr.h"
#include "heappage.h"
#include "record.h"
#include "dirpage.h"
#include "dir.h"
#include <cassert>
#include <iostream>
#include "heapfile.h"

void Test()
{
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

    std::size_t loopSize = 230;
    for( std::size_t i = 0; i < loopSize; i++ )
    {
        // EXPECT_NO_THROW( hf.Insert( rec ) );
        hf.Insert( rec );
    }

}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


