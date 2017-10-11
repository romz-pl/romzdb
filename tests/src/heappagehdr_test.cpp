#include <gtest/gtest.h>
#include <cstdlib>
#include <heappagehdr.h>
#include <record.h>

TEST(HeapPageHdr, Insert)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    Record rec( {'a'} );
    hdr.Insert( rec.GetLength() );
    EXPECT_EQ( hdr.GetSlotNo(), 1 );

}

TEST(HeapPageHdr, Delete)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    Record rec( {'a'} );
    hdr.Insert( rec.GetLength() );
    EXPECT_EQ( hdr.GetSlotNo(), 1 );

    hdr.Delete( 0 );
    EXPECT_EQ( hdr.GetSlotNo(), 0 );
}


TEST(HeapPageHdr, GetFreeSpace)
{
    HeapPageHdr hdr;

    std::size_t len = Page::PageSize - sizeof( std::size_t ) - 2 * sizeof( PageOffset );

    EXPECT_NO_THROW( hdr.Insert( len ) );
    EXPECT_EQ( hdr.GetFreeSpace(), 0 );

    hdr.Delete( 0 );
    EXPECT_EQ( hdr.GetFreeSpace(), Page::PageSize - sizeof( std::size_t ) );

    EXPECT_NO_THROW( hdr.Insert( len ) );
    EXPECT_EQ( hdr.GetFreeSpace(), 0 );
    EXPECT_ANY_THROW( hdr.Insert( 1 ) );


}

TEST(HeapPageHdr, FromToPage)
{
    Page page;
    HeapPageHdr hdrA, hdrB;

    hdrA.Insert( 1 );
    hdrA.Insert( 11 );
    hdrA.Insert( 111 );
    hdrA.ToPage( page );

    hdrB.FromPage( page );

    EXPECT_EQ( hdrA.GetSlotNo(), hdrB.GetSlotNo() );

    for( SlotId id = 0; id < hdrA.GetSlotNo(); id ++ )
    {
        EXPECT_EQ( hdrA.GetSlot( id ).m_length, hdrB.GetSlot( id ).m_length );
        EXPECT_EQ( hdrA.GetSlot( id ).m_offset, hdrB.GetSlot( id ).m_offset );
    }

}
