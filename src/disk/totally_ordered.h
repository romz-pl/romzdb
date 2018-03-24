#ifndef ROMZDB_LESS_THAN_COMPARABLE_H
#define ROMZDB_LESS_THAN_COMPARABLE_H

template < typename T >
struct less_than_comparable
{
     friend bool operator> ( const T& x, const T& y ) { return y < x; }
     friend bool operator<=( const T& x, const T& y ) { return !static_cast<bool>(y < x); }
     friend bool operator>=( const T& x, const T& y ) { return !static_cast<bool>(x < y); }
};

template < typename T >
struct equality_comparable
{
     friend bool operator!=( const T& x, const T& y ) { return !static_cast<bool>(x == y); }
};

template < typename T >
struct totally_ordered
    : less_than_comparable< T >
    , equality_comparable< T >
{

};


#endif
