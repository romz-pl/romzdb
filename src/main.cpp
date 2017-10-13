#include <iostream>

#include "buffermgr.h"
#include "heappage.h"
#include "record.h"
#include "dirpage.h"
#include <cassert>

void Test()
{
    Page page;
    DirPage dpA( page );

    const PageId pageId = 1;
    dpA.Insert( pageId );


    DirPage dpB( page );
    dpB.Insert( pageId );
}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


