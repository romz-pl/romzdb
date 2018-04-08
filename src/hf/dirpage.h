#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include <optional>
#include <vector>
#include "buffer/buffermgr.h"
#include "dirslot.h"
#include "page.h"
#include "recordid.h"


class DirPage : public Page
{
public:
    explicit DirPage( BufferMgr& buffer );
    DirPage( BufferMgr& buffer, PageId page_id );
    ~DirPage() = default;

    PageId get_next_page() const;
    void set_next_page( PageId id );

    std::optional< RecordId > insert_record( const Record &rec );
    bool remove_record( RecordId record_id );

    bool alloc_page();


    bool is_next_page() const;

    static std::uint32_t max_slot_no();

    std::uint32_t get_record_no() const;

    void get_all_records( std::vector< Record>& all ) const;


    class iterator
    {
    public:
        typedef std::ptrdiff_t  difference_type;
        typedef DirSlot         value_type;
        typedef DirSlot&        reference;
        typedef DirSlot*        pointer;

        typedef std::forward_iterator_tag iterator_category;

    public:
        iterator( DirSlot* slot ) : m_slot( slot )
        {
        }

        ~iterator() = default;

        iterator& operator++()
        {
            ++m_slot;
            return *this;
        }

        iterator operator++( int )
        {
            iterator retval = *this;
            ++( *this );
            return retval;
        }

        bool operator==( const iterator& other) const
        {
            return m_slot == other.m_slot;
        }

        bool operator!=( const iterator& other) const
        {
            return !( *this == other );
        }

        reference operator*() const
        {
            return *m_slot;
        }

        pointer operator->() const
        {
            return m_slot;
        }

    private:
        DirSlot* m_slot;


    };


    class const_iterator
    {
    public:
        typedef std::ptrdiff_t  difference_type;
        typedef DirSlot         value_type;
        typedef const DirSlot&  reference;
        typedef const DirSlot*  pointer;

        typedef std::forward_iterator_tag iterator_category;

    public:
        const_iterator( const DirSlot* slot ) : m_slot( slot )
        {
        }

        ~const_iterator() = default;

        const_iterator& operator++()
        {
            ++m_slot;
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
            return m_slot == other.m_slot;
        }

        bool operator!=( const const_iterator& other) const
        {
            return !( *this == other );
        }

        reference operator*() const
        {
            return *m_slot;
        }

        pointer operator->() const
        {
            return m_slot;
        }

    private:
        const DirSlot* m_slot;


    };

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;


private:
    void init( );
    DirSlot* get_slot();
    const DirSlot* get_slot() const;

private:
    enum Offset
    {
        Next_page = 0,
        Slot = Next_page + sizeof( PageId )
    };

private:

    static const PageId m_invalid_page_id;
};

#endif
