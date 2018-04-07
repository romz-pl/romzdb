#include <set>
#include "gtest/gtest.h"
#include "hf/heappage.h"
#include "util/temp_path.h"
#include "util/random_string.h"

class HeapPageFixture : public ::testing::Test
{
public:
   HeapPageFixture( )
       : m_db_file( nullptr )
       , m_space( nullptr )
       , m_buffer( nullptr )
       , m_hp( nullptr )
   {
       // initialization code here
   }

   void SetUp( )
   {
       const uint32_t max_size = ( 1U << 20 );
       m_db_file = new DbFile( ::get_temp_path(), max_size );
       m_space = new Space( *m_db_file );
       const std::size_t frameNo = 3;
       m_buffer = new BufferMgr( *m_space, frameNo );

       auto ret = m_buffer->alloc();
       m_buffer->unpin( ret.first, true );

       m_hp = new HeapPage( *m_buffer, ret.first );
   }

   void TearDown( ) {
       delete m_hp;
       delete m_buffer;
       delete m_space;
       delete m_db_file;
   }

   ~HeapPageFixture( )  {
       // cleanup any pending stuff, but no exceptions allowed
   }

   // put in any custom data members that you need
   DbFile *m_db_file;
   Space *m_space;
   BufferMgr *m_buffer;
   HeapPage *m_hp;
};


TEST_F(HeapPageFixture, insert_remove)
{
    const int record_no = 30;
    std::set< SlotId > sset;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( 5 );
        SlotId slot_id = m_hp->Insert( Record( str ) );
        sset.insert( slot_id );
    }


    std::uint32_t rem_no = 0, cnt = 0;
    for( auto v : sset )
    {
        if( cnt % 2 )
        {
            EXPECT_NO_THROW( m_hp->Remove( v ) );
            rem_no++;
        }
        cnt++;
    }

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( 5 );
        EXPECT_NO_THROW( m_hp->Insert( Record( str ) ) );
    }
}

TEST_F(HeapPageFixture, insert_too_long)
{
    const std::uint32_t count = 2 * DiskBlock::Size;
    const std::string str( count, 'A' );
    EXPECT_ANY_THROW( m_hp->Insert( Record( str ) ) );
}


TEST_F(HeapPageFixture, get)
{

    EXPECT_ANY_THROW( m_hp->Get( SlotId( 1 ) ) );


    std::map< SlotId, Record > mmap;

    const std::uint32_t count = 20;
    const int record_no = 30;
    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        const Record rec( str );
        const SlotId slot_id  = m_hp->Insert( rec );

        mmap.insert( std::make_pair( slot_id, rec ) );
    }

    for( auto v : mmap )
    {
        EXPECT_TRUE( m_hp->Get( v.first ) == v.second );
    }


    for( auto v : mmap )
    {
        EXPECT_NO_THROW( m_hp->Remove( v.first ) );
    }

    EXPECT_TRUE( m_hp->GetRecordNo() == 0 );

    for( auto v : mmap )
    {
        EXPECT_ANY_THROW( m_hp->Get( v.first ) );
    }
}
