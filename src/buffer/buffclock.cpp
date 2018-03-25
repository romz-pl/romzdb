#include "buffclock.h"

//
//
//
BuffClock::BuffClock( Space& space, std::uint32_t frame_no )
    : m_frame_no( frame_no )
    , m_clock_hand( frame_no - 1 )
    , m_frame( frame_no )
    , m_pool( frame_no )
    , m_space( space )
{

    for( std::size_t i = 0; i < m_frame.size(); i++ )
     {
         m_frame[ i ].m_frame_id = FrameId( i );
         m_frame[ i ].m_valid = false;
     }
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
    m_clock_hand.inc( m_frame_no );
}

//
// Allocate a free frame.
//
void BuffClock::allocBuff()
{
    //variables
    bool frameFreed = false;
    std::uint32_t countPinned = 0;
    // the frame hasn't been set and not all frames are pinned
    while( frameFreed == false && countPinned < m_frame_no )
    {
        advance();
        FrameClock& ff = m_frame[ m_clock_hand.to_uint32() ];

        if( ff.m_valid )
        {

            if( ff.m_refbit )
            {
                ff.m_refbit = false;
            }
            else
            {
                if( ff.m_pin_count != 0 )
                {
                    countPinned++;
                }
                else
                {
                    if( ff.m_dirty )
                    {
                        m_space.Write( m_pool[ m_clock_hand.to_uint32() ], ff.m_page_id );
                        //flush page to disk
                        // m_frame[ m_clock_hand ].file->writePage(bufPool[ m_clock_hand ]);
                        ff.m_dirty = false;
                    }
                    //dealloc
                    // hashTable->remove(m_frame[ m_clock_hand ].file, m_frame[ m_clock_hand ].pageNo);
                    m_map.erase( ff.m_page_id );
                    ff.clear();
                    frameFreed = true;
                    return;
                }
            }
        }
        else
        {
            //dealloc
            ff.clear();
            frameFreed = true;
            return;
        }
    }
    //if all pages are pinned throw excepton
    if( countPinned >= m_frame_no && frameFreed == false )
    {
        throw std::runtime_error( "BuffClock::allocBuff: Bufferis full" );
    }
}

//
// Reads the given page from the Space into a frame and returns the pointer to page.
// If the requested page is already present in the buffer pool pointer to that frame is returned
// otherwise a new frame is allocated from the buffer pool for reading the page.
//
DiskBlock* BuffClock::read( const PageId page_id )
{
    auto it = m_map.find( page_id );

    if( it != m_map.end() )
    {
        //look up was successful
        m_frame[ m_clock_hand.to_uint32() ].m_refbit = true;
        m_frame[ m_clock_hand.to_uint32() ].m_pin_count++;
        return &m_pool[ m_clock_hand.to_uint32() ];
    }

    //look up was unsucessful
    allocBuff();
    DiskBlock p = m_space.Read( page_id );
    m_pool[  m_clock_hand.to_uint32() ] = p;
    m_map.insert( std::make_pair( page_id, m_clock_hand ) );
    m_frame[ m_clock_hand.to_uint32() ].set( page_id );
    return &m_pool[ m_clock_hand.to_uint32() ];

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

    FrameClock& ff = m_frame[ m_clock_hand.to_uint32() ];

    if( ff.m_pin_count > 0 )
    {
        ff.m_pin_count--;
        if( dirty )
        {
            ff.m_dirty = true;
        }
    }
    else
    {
        throw std::runtime_error( "BuffClock::unpin: Page not pinned" );
    }

}

//
// Allocates a new, empty page in the file and returns the Page object.
// The newly allocated page is also assigned a frame in the buffer pool.
//
DiskBlock* BuffClock::alloc( PageId& page_id )
{
    page_id = m_space.Alloc();
    allocBuff();
    m_pool[ m_clock_hand.to_uint32() ] = m_space.Read( page_id );
    m_map.insert( std::make_pair( page_id, m_clock_hand ) );
    m_frame[ m_clock_hand.to_uint32() ].set( page_id );
    return &m_pool[ m_clock_hand.to_uint32() ];
}

void BuffClock::dispose( const PageId page_id )
{
    auto it = m_map.find( page_id );
    if( it == m_map.end() )
    {
        throw std::runtime_error( "BuffClock::dispose: Page is not in buffer" );
    }

    FrameClock& ff = m_frame[ m_clock_hand.to_uint32() ];
    if( !ff.m_valid )
    {
        ff.clear();
    }
    if( ff.m_dirty )
    {
        m_space.Write( m_pool[ m_clock_hand.to_uint32() ], page_id );
    }
    m_map.erase( page_id );
    m_frame[ m_clock_hand.to_uint32() ].clear();
    m_space.Dealloc( page_id );
}

//
// Writes out all dirty pages of the file to disk.
// All the frames assigned to the file need to be unpinned from buffer pool before this function can be successfully called.
// Otherwise Error returned.
//
void BuffClock::flush()
{
    uint32_t i = 0;
    //interate through bufdesctable
    for( auto& f : m_frame )
    {
        if( f.m_pin_count > 0)
        {
            throw std::runtime_error( "BuffClock::flush: Page is pinned" );
        }

        if( !f.m_valid )
        {
            continue;
        }

        if( f.m_dirty )
        {
            //flush page to disk
            m_space.Write( m_pool[ i ], f.m_page_id );
            f.m_dirty = false;
        }
        m_map.erase( f.m_page_id );
        //remove frame from hashtable
        // hashTable->remove(bufDescTable[i].file, bufDescTable[i].pageNo);
        f.clear();

        i++;
    }
}
