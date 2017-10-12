#include <gtest/gtest.h>
#include <cstdlib>
#include <heappagehdr.h>
#include <record.h>
#include <algorithm>
#include <random>

TEST(HeapPageHdr, Insert)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    Record rec( std::string("a") );
    const auto pair = hdr.Insert( rec.GetLength() );

    const auto offset = pair.first;
    EXPECT_EQ( offset, 0 );

    const auto slotId = pair.second;
    EXPECT_EQ( slotId, 0 );

    EXPECT_EQ( hdr.GetSlotNo(), 1 );

}

TEST(HeapPageHdr, Delete)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    Record rec( std::string( "a" ) );
    hdr.Insert( rec.GetLength() );
    EXPECT_EQ( hdr.GetSlotNo(), 1 );

    hdr.Delete( 0 );
    EXPECT_EQ( hdr.GetSlotNo(), 0 );
}

TEST(HeapPageHdr, DeleteMul)
{
    HeapPageHdr hdr;
    EXPECT_EQ( hdr.GetSlotNo(), 0 );

    const std::size_t recLength = 12;
    const std::size_t slotNo = 23;

    for( std::size_t id = 0; id < slotNo; id++ )
    {
        EXPECT_NO_THROW( hdr.Insert( recLength ) );
        EXPECT_EQ( hdr.GetSlotNo(), id + 1 );
    }

    EXPECT_TRUE( hdr.GetFreeSpace() > 0 );

    // Delete records in the random order
    std::random_device rd;
    std::mt19937 rng( rd() );

    for( std::size_t i = 0; i < slotNo; i++ )
    {
        std::uniform_int_distribution< SlotId > uni( 0 , hdr.GetSlotNo() - 1 );
        SlotId id = uni( rng );

        EXPECT_NO_THROW( hdr.Delete( id ) );
        EXPECT_EQ( hdr.GetSlotNo(), slotNo - 1 - i );
    }

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
