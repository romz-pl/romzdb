#ifndef ROMZDB_SCANER_H
#define ROMZDB_SCANER_H

#include "heapfile.h"
#include <iterator>

class Scaner
{
public:
    Scaner( const HeapFile& hf );
    ~Scaner() = default;

    class iterator : public std::iterator< std::forward_iterator_tag, RecordId >
    {
        using Iter = std::vector< RecordId >::iterator;
    public:
        explicit iterator( Iter it ) : m_it( it ) {}

        iterator& operator++() { m_it++; return *this; }
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
