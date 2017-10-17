#include <iostream>
#include "heapfile.h"
#include "dir.h"
#include "db.h"
#include <cassert>
#include <random>
#include <algorithm>
#include "scaner.h"




void Test()
{
    const std::string path = UnixFile::GetTempPath();
    // std::cout << path << "\n";
    std::size_t frameNo = 50;

    Db db( path, frameNo );
    HeapFile hf = db.CreteHeapFile();

    std::size_t loopSize = 20;
    for( std::size_t i = 0; i < loopSize; i++ )
    {
        const std::string txt = std::string( "A" ) + std::to_string( i );
        const Record recA( txt );
        const RecordId rid = hf.Insert( recA );
        std::cout << rid.GetPageId().GetValue() << "," << rid.GetSlotId().GetValue() << "  " << std::flush;
    }
    std::cout << "\n\n";

    Scaner scaner( hf );

    for( auto v : scaner )
    {
        std::cout << v.GetPageId().GetValue() << "," << v.GetSlotId().GetValue() << "  " << std::flush;
    }

    std::cout << "\n\n";

    for( auto v : scaner )
    {
        Record rec = hf.Get( v );
        std::string s = rec.ToString();
        std::cout << s << "  " << std::flush;
    }
}




int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


