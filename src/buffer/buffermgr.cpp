#include "buffermgr.h"
// #include <iostream>

//
//
//
BufferMgr::BufferMgr( Space& space, std::uint32_t frame_no )
    : m_frame( frame_no )
    , m_space( space )
{
    if( frame_no == 0 )
    {
        throw std::runtime_error( "BufferMgr::BufferMgr: Number of framses must be greater than zero" );
    }

    m_clock_hand = m_frame.data();
}

//
//
//
BufferMgr::~BufferMgr()
{
    flush();
}

//
// Advance clock to next frame in the buffer pool
//
void BufferMgr::advance_clock_hand()
{
    m_clock_hand++;
    if( m_clock_hand == m_frame.data() + m_frame.size() )
    {
        m_clock_hand = m_frame.data();
    }
}

//
// Allocate a free frame.
//
void BufferMgr::find_frame_for_replacement()
{
    std::uint32_t countPinned = 0;

    while( countPinned < m_frame.size() )
    {
        advance_clock_hand();

        if( m_clock_hand->is_for_replacement( m_space, m_map, countPinned ) )
        {
            return;
        }
    }

    //if all pages are pinned throw excepton
    throw std::runtime_error( "BufferMgr::allocBuff: Buffer is full" );
}

//
//
//
DiskBlock* BufferMgr::replace_frame( PageId page_id )
{
    find_frame_for_replacement();
    m_map.insert( std::make_pair( page_id, m_clock_hand ) );

    return m_clock_hand->read( m_space, page_id );
}

//
// Reads the given page from the Space into a frame and returns the pointer to page.
// If the requested page is already present in the buffer pool pointer to that frame is returned
// otherwise a new frame is allocated from the buffer pool for reading the page.
//
DiskBlock* BufferMgr::pin( PageId page_id )
{
    auto it = m_map.find( page_id );
    if( it != m_map.end() )
    {
        //look up was successful
        return m_clock_hand->pin();
    }

    //look up was unsucessful
    return replace_frame( page_id );
}

//
// Unpin a page from memory since it is no longer required for it to remain in memory.
//
void BufferMgr::unpin( PageId page_id, bool dirty )
{
    auto it = m_map.find( page_id );
    if( it == m_map.end() )
    {
        throw std::runtime_error( "BufferMgr::unpin: Page is not in buffer" );
    }

    it->second->unpin( dirty );
}

//
// Allocates a new, empty page in the file and returns the Page object.
// The newly allocated page is also assigned a frame in the buffer pool.
//
std::pair< PageId, DiskBlock* > BufferMgr::alloc()
{
    const PageId page_id = m_space.Alloc();
    DiskBlock* block = replace_frame( page_id );
    return std::make_pair( page_id, block );
}

//
// Delete page from file and also from buffer pool if present.
// Since the page is entirely deleted from file, its unnecessary to see if the page is dirty.
//
void BufferMgr::dispose( PageId page_id )
{
    auto it = m_map.find( page_id );
    if( it == m_map.end() )
    {
        throw std::runtime_error( "BufferMgr::dispose: Page is not in buffer" );
    }

    m_clock_hand->dispose( m_space, page_id );
    m_map.erase( page_id );
}

//
// Writes out all dirty pages of the file to disk.
// All the frames assigned to the file need to be unpinned from buffer pool before this function can be successfully called.
// Otherwise Error returned.
//
void BufferMgr::flush()
{
    for( auto& f : m_frame )
    {
        if( f.is_valid() )
            f.flush( m_space );
    }
}
