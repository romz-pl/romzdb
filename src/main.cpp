#include <iostream>
#include "heapfile.h"
#include "dir.h"
#include "db.h"


void Test()
{
    const std::string path = UnixFile::GetTempPath();
    const std::size_t frameNo = 10;
    Db db( path, frameNo );
    HeapFile hf = db.CreteHeapFile();


    Record rec( "A" );
    hf.Insert( rec );

}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


