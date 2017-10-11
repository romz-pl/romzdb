#include <gtest/gtest.h>
#include <cstdlib>
#include <heappagehdr.h>
#include <record.h>

TEST(HeapPageHdr, Insert)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    Record rec( {'a'} );
    hdr.Insert( rec );
    EXPECT_EQ( hdr.GetSlotNo(), 1 );

}

TEST(HeapPageHdr, Delete)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    Record rec( {'a'} );
    hdr.Insert( rec );
    EXPECT_EQ( hdr.GetSlotNo(), 1 );

    hdr.Delete( 0 );
    EXPECT_EQ( hdr.GetSlotNo(), 0 );
}


TEST(HeapPageHdr, FromToPage)
{
    Page page;
    HeapPageHdr hdrA, hdrB;


    hdrA.FromPage( page );
    hdrA.ToPage( page );
    hdrB.FromPage( page );

    // EXPECT_EQ( hdrA, hdrB );
}

