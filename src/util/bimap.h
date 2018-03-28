#include <map>
#include <cassert>

template< typename K, typename V >
class bimap
{
private:
    // KEY to VALUE mapping
    std::map< K, V > m_key;

    // VALUE to KEY mapping
    std::map< V, K > m_value;

public:
    bimap() = default;
    ~bimap() = default;

    bimap( const bimap& ) = default;
    bimap& operator=( const bimap& ) = default;

    bimap( bimap&& ) = default;
    bimap& operator=( bimap&& ) = default;

    bool erase_by_key( const K& key)
    {
        auto iter = m_key.find( key );
        if( iter == m_key.end() )
        {
            return false;
        }

        m_key.erase( iter );
        m_value.erase( iter->second );
        return true;
    }


    bool erase_by_value( const V& value )
    {
        auto iter = m_value.find( value );
        if( iter == m_value.end() )
        {
            return false;
        }

        m_value.erase( iter );
        m_key.erase( iter->second );
        return true;
    }

    bool insert( const K& key, const V& value )
    {
        assert( m_key.size() == m_value.size() );

        auto it_key = m_key.insert( std::make_pair( key, value ) );
        if ( !it_key.second )
        {
            return false;
        }

        auto it_map = m_value.insert( std::make_pair( value, key ) );
        if( !it_map.second )
        {
            m_key.erase( it_key.first );
            return false;
        }

        assert( m_key.size() == m_value.size() );
        return true;
    }

    K get_key( const V& value ) const
    {
        auto it = m_value.find( value );
        if( it == m_value.end() )
        {
            throw std::runtime_error("bimap::get_key_by_value: Cannot find value" );
        }
        return it->second;
    }

    V get_value( const K& key ) const
    {
        auto it = m_key.find( key );
        if( it == m_key.end() )
        {
            throw std::runtime_error("bimap::get_value_by_key: Cannot find value" );
        }
        return it->second;
    }

    void clear()
    {
        m_key.clear();
        m_value.clear();
    }

    bool empty() const noexcept
    {
        assert( m_key.empty() == m_value.empty() );
        return m_key.empty();
    }

    size_t size() const noexcept
    {
        assert( m_key.size() == m_value.size() );
        return m_key.size();
    }


    class const_iterator
    {
    private:
        typedef std::map< K, V > container;
        typename container::const_iterator m_it;

    public:
        typedef std::input_iterator_tag iterator_category;

        const_iterator( typename container::const_iterator it ) : m_it{ it }
        {

        }
        
        const_iterator& operator++()
        {
            ++m_it;
            return *this; 
        }

        const_iterator operator++( int )
        {
            const_iterator old = *this;
            ++( *this );
            return old;
        }

        typename container::value_type operator*() const
        {
            return *m_it;
        }
        
        typename container::const_pointer operator->() const
        {
            return m_it.operator->();
        }

        bool operator==( const const_iterator& rhs ) const
        {
            return m_it == rhs.m_it;
        }

        bool operator!=( const const_iterator& rhs ) const
        {
            return m_it != rhs.m_it;
        }
    };

    const_iterator begin() const noexcept
    {
        return const_iterator( m_key.cbegin() );
    }
    
    const_iterator end() const noexcept
    {
        return const_iterator( m_key.cend() );
    }

};

