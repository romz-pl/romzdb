// #include <unordered_map>
#include <map>
// #include <list>
#include <cassert>

template< typename K, typename V >
class bimap
{
    //typedef typename std::map< K, V >::iterator iter;
    // typedef typename std::map< K, V >::const_iterator const_iter;

    std::map< K, V > m_key_to_value;
    std::map< V, K > m_value_to_key;

public:
    bimap() = default;
    ~bimap() = default;

    bimap( const bimap& ) = default;
    bimap& operator=( const bimap& ) = default;

    bimap( bimap&& ) = default;
    bimap& operator=( bimap&& ) = default;

    bool erase_by_key( const K& key)
    {
        auto iter = m_key_to_value.find( key );
        if( iter == m_key_to_value.end() )
        {
            return false;
        }

        m_key_to_value.erase( iter );
        m_value_to_key.erase( iter->second );
        return true;
    }


    bool erase_by_value( const V& value )
    {
        auto iter = m_value_to_key.find( value );
        if( iter == m_value_to_key.end() )
        {
            return false;
        }

        m_value_to_key.erase( iter );
        m_key_to_value.erase( iter->second );
        return true;
    }

    bool insert( const K& key, const V& value )
    {
        assert( m_key_to_value.size() == m_value_to_key.size() );

        auto insert_to_key_map = m_key_to_value.insert( std::make_pair( key, value ) );
        if ( !insert_to_key_map.second )
        {
            return false;
        }

        auto insert_to_value_map = m_value_to_key.insert( std::make_pair( value, key ) );
        if( !insert_to_value_map.second )
        {
            m_key_to_value.erase( insert_to_key_map.first );
            return false;
        }

        assert( m_key_to_value.size() == m_value_to_key.size() );
        return true;
    }

    K get_key_by_value( const V& value ) const
    {
        auto it = m_value_to_key.find( value );
        if( it == m_value_to_key.end() )
        {
            throw std::runtime_error("bimap::get_key_by_value: Cannot find value" );
        }
        return it->second;
    }

    V get_value_by_key( const K& key ) const
    {
        auto it = m_key_to_value.find( key );
        if( it == m_key_to_value.end() )
        {
            throw std::runtime_error("bimap::get_value_by_key: Cannot find value" );
        }
        return it->second;
    }

    void clear()
    {
        m_key_to_value.clear();
        m_value_to_key.clear();
    }

    bool empty() const noexcept
    {
        assert( m_key_to_value.empty() == m_value_to_key.empty() );
        return m_key_to_value.empty();
    }

    size_t size() const noexcept
    {
        assert( m_key_to_value.size() == m_value_to_key.size() );
        return m_key_to_value.size();
    }


    class iterator
    {
        typedef typename std::map< K, V >::const_iterator const_iter;
        const_iter m_it;

    public:
        typedef std::input_iterator_tag iterator_category;

        iterator( const_iter it ) : m_it{ it }
        {

        }
        
        iterator& operator++()
        {
            ++m_it;
            return *this; 
        }

        iterator operator++( int )
        {
            iterator old = *this;
            ++( *this );
            return old;
        }

        typename const_iter::value_type operator*() const
        {
            return *m_it;
        }
        
        typename const_iter::pointer operator->() const
        {
            return m_it.operator->();
        }

        bool operator==( const iterator& rhs ) const
        {
            return m_it == rhs.m_it;
        }

        bool operator!=( const iterator& rhs ) const
        {
            return m_it != rhs.m_it;
        }
    };

    iterator begin() const noexcept
    {
        return iterator( m_key_to_value.cbegin() );
    }
    
    iterator end() const noexcept
    {
        return iterator( m_key_to_value.cend() );
    }

};

