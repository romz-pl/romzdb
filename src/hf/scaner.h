#ifndef ROMZDB_SCANER_H
#define ROMZDB_SCANER_H

#include "record.h"
#include "disk/pageid.h"
#include "heap_file.h"
#include "heappage.h"

/*

class Scaner
{
public:
    Scaner( const HeapFile& hf );


public:
    class const_iterator
    {
    public:
        typedef std::ptrdiff_t  difference_type;
        typedef Record          value_type;
        typedef const Record&   reference;
        typedef const Record*   pointer;

        typedef std::forward_iterator_tag iterator_category;

    public:
        const_iterator( PageId dir_page_id )
            : m_dir_page_id( dir_page_id )
        {
            m_dp = new DirPage( buffer, dir_page_id );
            m_dp_iterator = m_dp->begin();

            m_hp = HeapPage( buffer, m_dp_iterator->m_page_id );
            m_hp_iterator = m_hp->begin();
        }

        ~const_iterator()
        {
            delete m_dp;
            delete m_hp;
        }

        const_iterator& operator++()
        {
            while( true )
            {
                ++m_hp_iterator;
                if( m_hp_iterator == m_hp->end() )
                {
                    delete m_hp;
                    m_hp = nullptr;
                    break;
                }

                if( m_hp_iterator->IsValid() )
                {
                    return *this;
                }
            }

            while( true )
            {
                ++m_dp_iterator;
                if( m_dp_iterator == m_dp->end() )
                {
                    if( !m_dp->is_next_page() )
                    {
                        delete m_dp;
                        m_dp = nullptr;
                        return *this;
                    }
                    else
                    {
                        const PageId m_dir_page_id = m_dp->get_next_page();
                        delete m_dp;
                        m_dp = new DirPage( buffer, m_dir_page_id );
                        m_dp_iterator = m_dp->begin();
                    }
                }

                if( m_dp_iterator->m_valid() && !m_dp_iterator->is_empty() )
                {
                    m_hp = new HeapPage( buffer, m_dp_iterator->m_page_id );
                    m_hp_iterator = m_hp->begin();
                    return *this;
                }
            }


            return *this;
        }

        const_iterator operator++( int )
        {
            const_iterator retval = *this;
            ++( *this );
            return retval;
        }

        bool operator==( const const_iterator& other) const
        {
            return m_dir_page_id == other.m_dir_page_id;
        }

        bool operator!=( const const_iterator& other) const
        {
            return !( *this == other );
        }

        reference operator*() const
        {
            //return *m_slot;
        }

        pointer operator->() const
        {
            //return m_slot;
        }

    private:
        PageId m_dir_page_id;

        DirPage* m_dp;
        DirPage::iterator m_dp_iterator;

        HeapPage* m_hp;
        HeapPage::iterator m_hp_iterator;
    };

    const_iterator begin() const;
    const_iterator end() const;


private:
    const HeapFile& m_hf;
};
*/

#endif
