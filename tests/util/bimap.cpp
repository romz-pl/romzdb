#include "gtest/gtest.h"
#include "util/bimap.h"


TEST( bimap, empty )
{
    bimap< int, std::string > map;
    EXPECT_TRUE( map.size() == 0 );
    EXPECT_TRUE( map.empty() );
    EXPECT_NO_THROW( map.clear() );
}

TEST( bimap, one_element )
{
    bimap< int, int > map;
    map.insert( 1, 2 );
    EXPECT_EQ( 2, map.get_value( 1 ) );
    EXPECT_EQ( 1, map.get_key( 2 ) );
    EXPECT_EQ( 1, map.size() );
    EXPECT_NO_THROW( map.clear() );
}


TEST( bimap, multi_items )
{
    bimap< int, std::string > map;

    EXPECT_TRUE( map.insert( 10, "aba"  ) );
    EXPECT_TRUE( map.insert( 20, "caba" ) );
    EXPECT_TRUE( map.insert( 30, "xxyy" ) );
    EXPECT_FALSE( map.empty() );
    EXPECT_EQ( 3, map.size() );

    EXPECT_EQ( 10, map.get_key( "aba"  ) );
    EXPECT_EQ( 20, map.get_key( "caba" ) );
    EXPECT_EQ( 30, map.get_key( "xxyy" ) );
    EXPECT_FALSE( map.empty() );
    EXPECT_EQ( 3, map.size() );

    EXPECT_EQ( "aba" , map.get_value( 10 ) );
    EXPECT_EQ( "caba", map.get_value( 20 ) );
    EXPECT_EQ( "xxyy", map.get_value( 30 ) );
    EXPECT_FALSE( map.empty() );
    EXPECT_EQ( 3, map.size() );

    EXPECT_NO_THROW( map.clear() );
}

TEST( bimap, insert_element_with_existing_value_or_key )
{
    bimap< int, std::string > map;

    EXPECT_TRUE( map.insert( 1, "aba"  ) );
    EXPECT_TRUE( map.insert( 2, "caba" ) );
    EXPECT_TRUE( map.insert( 3, "xxyy" ) );

    EXPECT_FALSE( map.insert( 4, "aba" ) );
    EXPECT_FALSE( map.insert( 3, "QWE" ) );
    EXPECT_FALSE( map.insert( 2, "caba") );

    EXPECT_EQ( "aba" , map.get_value( 1 ) );
    EXPECT_EQ( "caba", map.get_value( 2 ) );
    EXPECT_EQ( "xxyy", map.get_value( 3 ) );

    EXPECT_EQ( 1, map.get_key("aba"  ) );
    EXPECT_EQ( 2, map.get_key("caba" ) );
    EXPECT_EQ( 3, map.get_key("xxyy" ) );

    EXPECT_EQ( 3, map.size() );
}


TEST( bimap, geting_absent_value )
{
    bimap< int, std::string > map;

    EXPECT_ANY_THROW( map.get_value( 1 ) );
    EXPECT_ANY_THROW( map.get_key( "asdasd" ) );
}


TEST( bimap, delete_existing_element )
{
    bimap< int, std::string > map;

    EXPECT_TRUE( map.insert( 10, "aba"  ) );
    EXPECT_TRUE( map.insert( 20, "caba" ) );
    EXPECT_TRUE( map.insert( 30, "xxyy" ) );

    EXPECT_TRUE( map.erase_by_key( 20 ) );
    EXPECT_ANY_THROW( map.get_value( 20 ) );
    EXPECT_ANY_THROW( map.get_key( "caba" ) );
    
    EXPECT_TRUE( map.erase_by_value( "aba" ) );
    EXPECT_ANY_THROW( map.get_value( 10 ) );
    EXPECT_ANY_THROW( map.get_key( "aba" ) );
    
    EXPECT_EQ( 30, map.get_key( "xxyy" ) );

    EXPECT_EQ( 1, map.size() );
    EXPECT_FALSE( map.empty() );
}


TEST( bimap, delete_absent_element )
{
    bimap< int, std::string > map;
    EXPECT_TRUE( map.insert( 1, "aba" ) );

    EXPECT_FALSE( map.erase_by_key( 213123 ) );
    EXPECT_FALSE( map.erase_by_key( 131451 ) );

    EXPECT_FALSE( map.erase_by_value( "asdq" ) );
    EXPECT_FALSE( map.erase_by_value( "asdh" ) );
    
    EXPECT_EQ( 1, map.size() );
    EXPECT_FALSE( map.empty() );
}


TEST( bimap, random_insert_erase )
{
    bimap< int, int > map;
    std::set< int > keys_set;
    std::set< int > values_set;

    for( std::size_t i = 0; i < 1000; ++i )
    {
        int key = std::rand(); 
        int value = std::rand();
        auto in = map.insert( key, value );
        EXPECT_TRUE(in == ( keys_set.find( key) == keys_set.end() ) );
        EXPECT_TRUE(in == ( values_set.find( value) == values_set.end() ) );
        keys_set.insert( key );
        values_set.insert( value );
    }

    for( std::size_t i = 0; i < 1000; ++i )
    {
        int key = std::rand(); 
        auto del = map.erase_by_key( key );
        EXPECT_TRUE( del == ( keys_set.find( key ) != keys_set.end() ) );
        keys_set.erase( key );
    }

    EXPECT_EQ( keys_set.size(), map.size() );
}


TEST( bimap, copys_independence )
{
    bimap< int, std::string > a;
    EXPECT_TRUE( a.insert( 1, "xxyy" ) );
    EXPECT_TRUE( a.insert( 2, "abaa" ) );
    EXPECT_TRUE( a.insert( 3, "abac" ) );
    bimap< int, std::string > b = a;

    EXPECT_TRUE( a.erase_by_value( "xxyy" ) );
    EXPECT_TRUE( a.erase_by_key( 3 ) );

    EXPECT_TRUE( b.erase_by_key( 1 ) );
    EXPECT_TRUE( b.erase_by_value( "abaa" ) );

    EXPECT_ANY_THROW( b.get_value( 1 ) );
    EXPECT_ANY_THROW( b.get_value( 2 ) );
    EXPECT_ANY_THROW( b.get_key( "xxyy" ) );
    EXPECT_ANY_THROW( b.get_key( "abaa" ) );
    
    EXPECT_EQ( "abac", b.get_value( 3 ) );
    EXPECT_EQ( 3, b.get_key( "abac" ) );
    EXPECT_EQ( 1, b.size() );

    EXPECT_EQ( "abaa", a.get_value( 2 ) );
    EXPECT_EQ( 1, a.size() );
}


TEST( bimap, assiginment_independence )
{
    bimap< int, std::string > a;
    EXPECT_TRUE( a.insert( 1, "xxyy" ) );
    EXPECT_TRUE( a.insert( 2, "abaa" ) );
    EXPECT_TRUE( a.insert( 3, "abac" ) );
    bimap< int, std::string > b;
    b = a;

    EXPECT_TRUE( a.erase_by_value( "xxyy" ) );
    EXPECT_TRUE( a.erase_by_key( 3 ) );

    EXPECT_TRUE( b.erase_by_key( 1 ) );
    EXPECT_TRUE( b.erase_by_value( "abaa" ) );

    EXPECT_ANY_THROW( b.get_value( 1 ) );
    EXPECT_ANY_THROW( b.get_value( 2 ) );
    EXPECT_ANY_THROW( b.get_key( "xxyy" ) );
    EXPECT_ANY_THROW( b.get_key( "abaa" ) );
    
    EXPECT_EQ( "abac", b.get_value( 3 ) );
    EXPECT_EQ( 3, b.get_key( "abac" ) );
    EXPECT_EQ( 1, b.size() );

    EXPECT_EQ( "abaa", a.get_value( 2 ) );
    EXPECT_EQ( 1, a.size() );
}


TEST( bimap, iterate_over_collection )
{
    bimap< int, int > map;
    std::set< int > keys_set;
    std::set< int > values_set;
    std::set< std::pair< int, int > > key_value_pairs;

    for( size_t i = 0; i < 1000; ++i )
    {
        int key = std::rand(); 
        int value = std::rand();
        if( map.insert( key, value ) )
        {
            keys_set.insert( key );
            values_set.insert( value );
            key_value_pairs.insert( std::make_pair( key, value ) );
        }
    }

    for( auto pair : map )
    {
        EXPECT_TRUE( key_value_pairs.find( pair ) != key_value_pairs.end() );
    }

    for( auto i = map.begin(); i != map.end(); i++ )
    {
        EXPECT_TRUE( key_value_pairs.find( *i ) != key_value_pairs.end() );
        EXPECT_TRUE( map.get_value( i->first ) );
        EXPECT_TRUE( map.get_key( i->second ) );

        key_value_pairs.erase( *i );
    }

    EXPECT_EQ( 0, key_value_pairs.size() );
}

