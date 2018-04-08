#ifndef ROMZDB_SCANER_H
#define ROMZDB_SCANER_H

#include "record.h"
#include "disk/pageid.h"
#include "heap_file.h"


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
        const_iterator( PageId dir_page_id ) : m_dir_page_id( dir_page_id )
        {
        }

        ~const_iterator() = default;

        const_iterator& operator++()
        {
            //++m_slot;

            ++m_dir_page_iterator;
            if( m_dir_page_iterator == m_dp->end() )
            {
                if( !m_dp->is_next_page() )
                {
                    delete m_dp;
                    return const_iterator;
                }
                m_dir_page_id = m_dp->get_next_page();

                delete m_dp;
                m_dp = new DirPage( m_dir_page_id );
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
        DirPage::iterator m_dir_page_iterator;
    };

    const_iterator begin() const;
    const_iterator end() const;


private:
    const HeapFile& m_hf;
};
*/

#endif
