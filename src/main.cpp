#include <iostream>
#include "heappage.h"
#include "dirpage.h"

void Test()
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );


    DirPage dp( bufferMgr, pageId );

    const Record rec( "ABC" );
    ( dp.Insert( rec ).first );

    pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );
    dp.InsertPage( pageId );

    ( dp.Insert( rec ).first );

    auto reca = dp.Insert( Record( "a" ) );
    ( reca.first );

    auto recb = dp.Insert( Record( "b" ) );
    ( recb.first );

    ( dp.Delete( reca.second ) );
    ( dp.Delete( recb.second ) );

    ( dp.Delete( recb.second ) );
    ( dp.Delete( reca.second ) );

}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


