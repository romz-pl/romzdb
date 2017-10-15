#include <iostream>
#include "heappage.h"

void Test()
{
    UnixFile uf( UnixFile::GetTempPath(), UnixFile::Mode::Create );
    DiskSpaceMgr ds( uf );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( ds, frameNo );


    const PageId pageId = bufferMgr.GetNew().first;
    bufferMgr.Unpin( pageId );

    HeapPage hp( bufferMgr, pageId );

    for( int i = 0; i < 10; i++ )
    {
        std::string str( "abc:" );
        str += std::to_string( i );
        Record rec( str );
        SlotId slotId = hp.Insert( rec );
        // The page is NOT empty
        hp.Get( slotId );
        Record ret = hp.Get( slotId );
        // EXPECT_EQ( rec, ret );
    }
}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


