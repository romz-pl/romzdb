#include <set>
#include "gtest/gtest.h"
#include "hf/dirpage.h"
#include "util/temp_path.h"
#include "hf/heappage.h"

class DirPageFixture : public ::testing::Test
{
public:
   DirPageFixture( )
       : m_db_file( nullptr )
       , m_space( nullptr )
       , m_buffer( nullptr )
       , m_dp( nullptr )
   {
       // initialization code here
   }

   void SetUp( )
   {
       const uint32_t max_size = ( 1U << 24 );
       m_db_file = new DbFile( ::get_temp_path(), max_size );
       m_space = new Space( *m_db_file );
       const std::size_t frameNo = 3;
       m_buffer = new BufferMgr( *m_space, frameNo );
       m_dp = new DirPage( *m_buffer);
   }

   void TearDown( )
   {
       delete m_dp;
       delete m_buffer;
       delete m_space;
       delete m_db_file;
   }

   ~DirPageFixture( )
   {
       // cleanup any pending stuff, but no exceptions allowed
   }

public:
   // put in any custom data members that you need
   DbFile *m_db_file;
   Space *m_space;
   BufferMgr *m_buffer;
   DirPage *m_dp;
};

TEST_F(DirPageFixture, get_set_next_page)
{
    const PageId page_id( 3, 1 );
    m_dp->set_next_page( page_id );

    EXPECT_TRUE( m_dp->get_next_page() == page_id );

}

TEST_F(DirPageFixture, alloc_dispose_page)
{
    auto ret = m_buffer->alloc();
    const PageId page_id = ret.first;
    m_buffer->unpin( page_id, true );

    EXPECT_TRUE( m_dp->alloc_page( page_id ) );
    EXPECT_TRUE( m_dp->dispose_page( page_id ) );
    EXPECT_FALSE( m_dp->dispose_page( page_id ) );
}


TEST_F(DirPageFixture, insert_remove_record)
{
    const Record rec( "aa" );

    EXPECT_FALSE( m_dp->insert_record( rec ).has_value() );

    auto ret = m_buffer->alloc();
    m_buffer->unpin( ret.first, true );
    m_dp->alloc_page( ret.first );

    const int record_no = DirPage::max_slot_no() / 3;
    for( int i = 0; i < record_no; i++ )
    {
        EXPECT_TRUE( m_dp->insert_record( rec ).has_value() );
    }

    std::set< RecordId > sset;
    for( int i = 0; i < record_no; i++ )
    {
        std::optional< RecordId > page_id = m_dp->insert_record( rec );
        EXPECT_TRUE( page_id.has_value() );
        sset.insert( page_id.value() );
    }

    for( auto v : sset )
    {
        EXPECT_TRUE( m_dp->remove_record( v ) );
    }
}

/*
TEST_F(DirPageFixture, insert_failure)
{
    const std::uint32_t count = HeapPage::GetMaxRecordLength();

    EXPECT_FALSE( m_dp->insert_record( count ).has_value() );

    const int record_no = DirPage::max_slot_no();
    for( int i = 0; i < record_no; i++ )
    {
        auto ret = m_buffer->alloc();
        m_buffer->unpin( ret.first, true );
        EXPECT_TRUE( m_dp->alloc_page( ret.first ) );

        EXPECT_TRUE( m_dp->insert_record( count ) );
    }

    EXPECT_FALSE( m_dp->insert_record( count ) );
}
*/


TEST_F(DirPageFixture, insert_record_too_long)
{
    const std::string str( 2 * DiskBlock::Size, 'A' );
    const Record rec( str );
    EXPECT_ANY_THROW( m_dp->insert_record( rec ) );
}


