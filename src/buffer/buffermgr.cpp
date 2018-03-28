#include "buffermgr.h"

//
//
//
BufferMgr::BufferMgr( Space& space, std::uint32_t frame_no )
    : m_pool( frame_no )
    , m_space( space )
{
    if( frame_no == 0 )
    {
        throw std::runtime_error( "BufferMgr::BufferMgr: Number of framses must be greater than zero" );
    }

    m_clock_hand = m_pool.data();

    for( std::size_t i = 0; i < m_pool.size(); i++ )
    {
        m_free.push( m_pool.data() + i );
    }
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
    if( m_clock_hand == m_pool.data() + m_pool.size() )
    {
        m_clock_hand = m_pool.data();
    }
}

//
// Move clock hand to the free frame.
//
void BufferMgr::find_frame_for_replacement()
{
    if( !m_free.empty() )
    {
        m_clock_hand = m_free.top();
        m_free.pop();
        return;
    }

    std::uint32_t countPinned = 0;

    while( countPinned < m_pool.size() )
    {
        advance_clock_hand();

        if( m_clock_hand->is_for_replacement( countPinned ) )
        {
            const PageId page_id = m_bimap.find_key( m_clock_hand ).value();
            m_clock_hand->write( m_space, page_id );
            m_bimap.erase_by_value( m_clock_hand );
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

    DiskBlock *block = m_clock_hand->read( m_space, page_id );
    m_bimap.insert( page_id, m_clock_hand );

    return block;
}

//
// Reads the given page from the Space into a frame and returns the pointer to page.
// If the requested page is already present in the buffer pool pointer to that frame is returned
// otherwise a new frame is allocated from the buffer pool for reading the page.
//
DiskBlock* BufferMgr::pin( PageId page_id )
{
    auto it = m_bimap.find_value( page_id );
    if( it.has_value() )
    {
        Frame* f = it.value();
        assert( f );
        return f->pin();
    }

    return replace_frame( page_id );
}

//
// Unpin a page from memory since it is no longer required for it to remain in memory.
//
void BufferMgr::unpin( PageId page_id, bool dirty )
{
    auto it = m_bimap.find_value( page_id );
    if( !it.has_value() )
    {
        throw std::runtime_error( "BufferMgr::unpin: Page is not in buffer" );
    }

    Frame* f = it.value();
    assert( f );
    f->unpin( dirty );
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
    auto it = m_bimap.find_value( page_id );
    if( !it.has_value() )
    {
        throw std::runtime_error( "BufferMgr::dispose: Page is not in buffer" );
    }

    Frame* f = it.value();
    assert( f );
    f->dispose( m_space, page_id );

    m_free.push( f );
    m_bimap.erase_by_key( page_id );
}

//
// Writes out all dirty pages of the file to disk.
// All the frames assigned to the file need to be unpinned from buffer pool before this function can be successfully called.
// Otherwise Error returned.
//
void BufferMgr::flush()
{
    for( auto it : m_bimap )
    {
        assert( it.second );
        it.second->flush( m_space, it.first );
    }
}
