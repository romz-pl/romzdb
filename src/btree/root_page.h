#ifndef ROMZDB_ROOT_PAGE_H
#define ROMZDB_ROOT_PAGE_H

#include <optional>
#include <vector>
#include "buffer/buffermgr.h"
//#include "dirslot.h"
#include "hf/page.h"
//#include "recordid.h"

#include "hf/slot.h"
#include "hf/slotid.h"

class root_page : public Page
{

public:
    explicit root_page( BufferMgr& buffer );
    root_page( BufferMgr& buffer, PageId page_id );
    ~root_page() = default;


    class iterator
    {
    public:
        typedef std::ptrdiff_t  difference_type;
        typedef Slot            value_type;
        typedef Slot&           reference;
        typedef Slot*           pointer;

        typedef std::bidirectional_iterator_tag iterator_category;

    public:
        iterator( Slot* slot ) : m_slot( slot )
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

        iterator& operator--()
        {
            --m_slot;
            return *this;
        }

        iterator operator--( int )
        {
            iterator retval = *this;
            --( *this );
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
        Slot* m_slot;


    };

    class const_iterator
    {
    public:
        typedef std::ptrdiff_t  difference_type;
        typedef Slot            value_type;
        typedef const Slot&     reference;
        typedef const Slot*     pointer;

        typedef std::bidirectional_iterator_tag iterator_category;

    public:
        const_iterator( const Slot* slot ) : m_slot( slot )
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

        const_iterator& operator--()
        {
            --m_slot;
            return *this;
        }

        const_iterator operator--( int )
        {
            const_iterator retval = *this;
            --( *this );
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
        const Slot* m_slot;


    };

    typedef std::reverse_iterator< iterator >       reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;


    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;




private:

    Slot get_slot( SlotId slotId ) const;
    void check_slot_id( SlotId slotIdEx ) const;

    std::uint16_t get_free_space() const;
    void set_free_space( std::uint16_t v );

    std::uint16_t get_slot_no() const;
    void set_slot_no( std::uint16_t v );

    Slot* get_slot_array();
    const Slot* get_slot_array() const;

    Slot* get_slot( std::uint16_t slot_id );
    const Slot* get_slot( std::uint16_t slot_id ) const;

    void shift_data( std::uint16_t offset, std::uint16_t length );
    void decrease_slot_offset( std::uint16_t offset, std::uint16_t length );
    void remove_slots();



    enum Offset
    {
        Free_space = DiskBlock::Size - sizeof( std::uint16_t ),
        Slot_no = Free_space - sizeof( std::uint16_t ),
        Slot_array = Slot_no - sizeof( Slot )

    };
};

#endif
