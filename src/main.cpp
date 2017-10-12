#include <iostream>

#include "buffermgr.h"
#include "heappage.h"
#include "record.h"
#include <cassert>

void Test()
{
    Record rec( {'a', 'b', 'c'} );
    Page page;
    HeapPage hp( page );

    SlotId slotId = hp.Insert( rec );
    // The page is NOT empty
   hp.Get( slotId );
}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


