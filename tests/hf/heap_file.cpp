#include <set>
#include "gtest/gtest.h"
#include "hf/heap_file.h"
#include "util/temp_path.h"
#include "util/random_string.h"

class HeapFileFixture : public ::testing::Test
{
public:
   HeapFileFixture( )
       : m_db_file( nullptr )
       , m_space( nullptr )
       , m_buffer( nullptr )
       , m_hf( nullptr )
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
       m_hf = new HeapFile( *m_buffer);
   }

   void TearDown( )
   {
       delete m_hf;
       delete m_buffer;
       delete m_space;
       delete m_db_file;
   }

   ~HeapFileFixture( )
   {
       // cleanup any pending stuff, but no exceptions allowed
   }

public:
   // put in any custom data members that you need
   DbFile *m_db_file;
   Space *m_space;
   BufferMgr *m_buffer;
   HeapFile *m_hf;
};


TEST(HeapFile, open)
{
    const uint32_t max_size = ( 1U << 20 );
    DbFile db_file( ::get_temp_path(), max_size );
    Space space( db_file );
    const std::size_t frameNo = 3;
    BufferMgr bufferMgr( space, frameNo );

    HeapFile hf_a{ bufferMgr };
    const PageId header_page = hf_a.get_header_page();

    EXPECT_NO_THROW( HeapFile( bufferMgr, header_page ) );
}



TEST_F(HeapFileFixture, insert_remove)
{
    const std::uint32_t count = 2000;
    const int record_no = 3000;
    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        EXPECT_NO_THROW( m_hf->insert( Record( str ) ) );
    }

    std::set< RecordId > sset;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( 200 );
        RecordId record_id = m_hf->insert( Record( str ) );
        sset.insert( record_id );
    }

    EXPECT_TRUE( m_hf->get_record_no() == 2 * record_no );

    std::uint32_t rem_no = 0, cnt = 0;
    for( auto v : sset )
    {
        if( cnt % 2 )
        {
            EXPECT_NO_THROW( m_hf->remove( v ) );
            rem_no++;
        }
        cnt++;
    }

    EXPECT_TRUE( m_hf->get_record_no() == 2 * record_no - rem_no );

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( 10 );
        EXPECT_NO_THROW( m_hf->insert( Record( str ) ) );
    }

    EXPECT_TRUE( m_hf->get_record_no() == 3 * record_no - rem_no );
}

TEST_F(HeapFileFixture, insert_too_long)
{
    const std::uint32_t count = 2 * DiskBlock::Size;
    const std::string str( count, 'A' );
    EXPECT_ANY_THROW( m_hf->insert( Record( str ) ) );
}

TEST_F(HeapFileFixture, insert_fixed_length)
{
    const int record_no = 300;
    std::set< RecordId > sset;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str( 1000, 'A' );
        RecordId record_id = m_hf->insert( Record( str ) );
        sset.insert( record_id );
    }

    for( auto v : sset )
    {
        EXPECT_NO_THROW( m_hf->remove( v ) );
    }
}

TEST_F(HeapFileFixture, get)
{
    std::map< RecordId, Record > mmap;

    const std::uint32_t count = 20;
    const int record_no = 3000;
    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        const Record rec( str );
        const RecordId record_id  = m_hf->insert( rec );

        mmap.insert( std::make_pair( record_id, rec ) );
    }

    EXPECT_TRUE( m_hf->get_record_no() == record_no );

    for( auto v : mmap )
    {
        EXPECT_TRUE( m_hf->get( v.first ) == v.second );
    }


    for( auto v : mmap )
    {
        EXPECT_NO_THROW( m_hf->remove( v.first ) );
    }

    EXPECT_TRUE( m_hf->get_record_no() == 0 );

    for( auto v : mmap )
    {
        EXPECT_ANY_THROW( m_hf->get( v.first ) );
    }
}

TEST_F(HeapFileFixture, content)
{
    std::map< RecordId, Record > smap;

    const std::uint32_t count = 200;
    const int record_no = 5000;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        const Record rec( str );
        RecordId record_id = m_hf->insert( rec );
        smap.insert( std::make_pair( record_id, rec ) );
    }

    EXPECT_TRUE( m_hf->get_record_no() == smap.size() );

    for( auto v : smap )
    {
       const Record rec = m_hf->get( v.first );
       EXPECT_TRUE( rec == v.second );
    }

    // Remove some random elements
    for( int i = 0; i < record_no / 2; i++ )
    {
        auto it = smap.begin();
        std::advance( it, rand() % smap.size() );

        m_hf->remove( it->first );
        smap.erase( it );
    }

    EXPECT_TRUE( m_hf->get_record_no() == smap.size() );

    for( auto v : smap )
    {
       const Record rec = m_hf->get( v.first );
       EXPECT_TRUE( rec == v.second );
    }
}

TEST_F(HeapFileFixture, remove_non_existing)
{
    const RecordId rid ( PageId( 0, 0 ), SlotId( 0 ) );
    EXPECT_ANY_THROW( m_hf->remove( rid ) );
}

TEST_F(HeapFileFixture, record)
{
    std::vector< Record > vec;

    const std::uint32_t count = 200;
    const int record_no = 50000;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        const Record rec( str );
        EXPECT_NO_THROW( m_hf->insert( rec ) );
        vec.push_back( rec );
    }

    std::vector< Record > dump;
    EXPECT_NO_THROW( m_hf->get_all_records( dump ) );

    std::sort( vec.begin(), vec.end() );
    std::sort( dump.begin(), dump.end() );

    EXPECT_TRUE( vec == dump );
}
