#ifndef ROMZDB_SCANER_H
#define ROMZDB_SCANER_H

#include "heap_file.h"
#include "recordid.h"

class Scaner
{
public:
    Scaner( const HeapFile& hf );
    ~Scaner() = default;

    class iterator
    {

        using Iter = std::vector< RecordId >::iterator;
    public:

        typedef std::ptrdiff_t  difference_type;
        typedef RecordId         value_type;
        typedef RecordId&        reference;
        typedef RecordId*        pointer;

        typedef std::forward_iterator_tag iterator_category;
    public:
        explicit iterator( Iter it ) : m_it( it ) {}

        iterator& operator++() { ++m_it; return *this; }
        iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

        bool operator== ( iterator v ) const { return m_it == v.m_it; }
        bool operator!= ( iterator v ) const { return m_it != v.m_it;}

        reference operator*() const { return *m_it; }

    private:
        Iter m_it;
    };

    iterator begin() { return iterator( m_rid.begin() ); }
    iterator end() { return iterator( m_rid.end() ); }

private:
    std::vector< RecordId > m_rid;
};

#endif
