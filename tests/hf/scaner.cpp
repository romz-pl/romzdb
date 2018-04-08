#include <set>
#include "gtest/gtest.h"
#include "hf/scaner.h"
#include "util/temp_path.h"
#include "util/random_string.h"

class ScanerFixture : public ::testing::Test
{
public:
   ScanerFixture( )
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

   ~ScanerFixture( )
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




TEST_F(ScanerFixture, iterate)
{
    const std::uint32_t count = 200;
    const int record_no = 50000;

    std::set< RecordId > sset;

    for( int i = 0; i < record_no; i++ )
    {
        const std::string str = random_string( count );
        RecordId record_id = m_hf->insert( Record( str ) );
        sset.insert( record_id );
    }

    Scaner scaner( *m_hf );

    for( auto it = scaner.begin(); it != scaner.end(); ++it )
    {
        EXPECT_TRUE( sset.find( *it ) != sset.end() );
    }

    for( auto v : scaner )
    {
        EXPECT_TRUE( sset.find( v ) != sset.end() );
    }
}
