#include "buffclock.h"

//
//
//
BuffClock::BuffClock( Space& space, std::uint32_t frame_no )
    : m_frame( frame_no )
    , m_space( space )
{
    m_clock_hand = m_frame.data();
}

//
//
//
BuffClock::~BuffClock()
{
    flush();
}

//
// Advance clock to next frame in the buffer pool
//
void BuffClock::advance()
{
    m_clock_hand++;
    if( m_clock_hand == m_frame.data() + m_frame.size() )
        m_clock_hand = m_frame.data();
}

//
// Allocate a free frame.
//
void BuffClock::allocBuff()
{
    std::uint32_t countPinned = 0;
    // the frame hasn't been set and not all frames are pinned
    while( countPinned < m_frame.size() )
    {
        advance();

        if( !m_clock_hand->m_valid )
        {
            return;
        }

        if( m_clock_hand->m_refbit )
        {
            m_clock_hand->m_refbit = false;
            continue;
        }

        if( m_clock_hand->m_pin_count != 0 )
        {
            countPinned++;
            continue;
        }

        m_clock_hand->write( m_space );
        m_map.erase( m_clock_hand->m_page_id );
        m_clock_hand->clear();
        return;
    }


    //if all pages are pinned throw excepton
    throw std::runtime_error( "BuffClock::allocBuff: Buffer is full" );

}

//
// Reads the given page from the Space into a frame and returns the pointer to page.
// If the requested page is already present in the buffer pool pointer to that frame is returned
// otherwise a new frame is allocated from the buffer pool for reading the page.
//
DiskBlock* BuffClock::get( const PageId page_id )
{
    auto it = m_map.find( page_id );

    if( it != m_map.end() )
    {
        //look up was successful
        return m_clock_hand->pin();
    }

    //look up was unsucessful
    allocBuff();
    m_map.insert( std::make_pair( page_id, m_clock_hand ) );

    return m_clock_hand->read( m_space, page_id );
}

//
// Unpin a page from memory since it is no longer required for it to remain in memory.
//
void BuffClock::unpin( const PageId page_id, const bool dirty )
{
    auto it = m_map.find( page_id );
    if( it == m_map.end() )
    {
        throw std::runtime_error( "BuffClock::unpin: Page is not in buffer" );
    }

    it->second->unpin( dirty );
}

//
// Allocates a new, empty page in the file and returns the Page object.
// The newly allocated page is also assigned a frame in the buffer pool.
//
std::pair< PageId, DiskBlock* > BuffClock::alloc()
{
    const PageId page_id = m_space.Alloc();
    allocBuff();
    m_clock_hand->m_block = m_space.Read( page_id );
    m_map.insert( std::make_pair( page_id, m_clock_hand ) );
    m_clock_hand->set( page_id );
    return std::make_pair( page_id, &( m_clock_hand->m_block ) );
}

//
// Delete page from file and also from buffer pool if present.
// Since the page is entirely deleted from file, its unnecessary to see if the page is dirty.
//
void BuffClock::dispose( const PageId page_id )
{
    auto it = m_map.find( page_id );
    if( it == m_map.end() )
    {
        throw std::runtime_error( "BuffClock::dispose: Page is not in buffer" );
    }

    m_clock_hand->dispose( m_space, page_id );
    m_map.erase( page_id );
}

//
// Writes out all dirty pages of the file to disk.
// All the frames assigned to the file need to be unpinned from buffer pool before this function can be successfully called.
// Otherwise Error returned.
//
void BuffClock::flush()
{
    for( auto& f : m_frame )
    {
        f.flush( m_space );
    }
}
