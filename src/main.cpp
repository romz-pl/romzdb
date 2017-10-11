#include <iostream>

#include "buffermgr.h"
#include "heappagehdr.h"
#include "record.h"
#include <cassert>

void Test()
{
    Page page;
    HeapPageHdr hdrA, hdrB;

    hdrA.Insert( 1 );
    hdrA.Insert( 11 );
    hdrA.Insert( 111 );
    hdrA.ToPage( page );

    hdrB.FromPage( page );

    assert( hdrA.GetSlotNo() == hdrB.GetSlotNo() );

    for( SlotId id = 0; id < hdrA.GetSlotNo(); id ++ )
    {
        assert( hdrA.GetSlot( id ).m_length == hdrB.GetSlot( id ).m_length );
        assert( hdrA.GetSlot( id ).m_offset == hdrB.GetSlot( id ).m_offset );
    }
}


int main()
{
    Test();
    std::cout << "OK\n";
    return 0;
}


