#include <set>
#include "gtest/gtest.h"
#include "hf/dirpage.h"
#include "util/temp_path.h"
#include "util/random_string.h"

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
}


TEST_F(DirPageFixture, insert_remove_record)
{
    const std::uint32_t count = 20;

    EXPECT_FALSE( m_dp->insert_record( count ).has_value() );

    auto ret = m_buffer->alloc();
    m_buffer->unpin( ret.first, true );
    m_dp->alloc_page( ret.first );

    const int record_no = 30;
    for( int i = 0; i < record_no; i++ )
    {
        EXPECT_NO_THROW( m_dp->insert_record( count ) );
    }

    std::set< PageId > sset;
    for( int i = 0; i < record_no; i++ )
    {
        std::optional< PageId > page_id = m_dp->insert_record( count );
        EXPECT_TRUE( page_id.has_value() );
        sset.insert( page_id.value() );
    }

    for( auto v : sset )
    {
        EXPECT_NO_THROW( m_dp->remove_record( v, count ) );
    }
}


TEST_F(DirPageFixture, insert_record_too_long)
{
    const std::uint32_t count = 2 * DiskBlock::Size;
    EXPECT_ANY_THROW( m_dp->insert_record( count ) );
}


