#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "buffermgr.h"

//
// Constructor
// frameNo - number of frames in the buffer manager.
//
BufferMgr::BufferMgr( Space &space, std::size_t frameNo )
    : m_space( space )
    , m_pool( frameNo )
{

}

//
//
//
BufferMgr::~BufferMgr()
{
    Flush();
}

//
// Returns  a pointer to a disk block pinned in the buffer.
//
//
// 1. Check the buffer pool to see if it contains the requested page.
//    If the page is already in the buffer:
//        a) (re)pin the page,
//        b) return a pointer to it.
//
//
// 2. When the page is not in the buffer pool, do the following:
//    a) Choose a frame for replacement, using the repacement policy.
//    b) Increment its "pin count"
//    c) If its "dirty bit" is on, write the page it contains to disk
//       (that is, the disk copy of the page is overwritten with the contents of the frame).
//    d) Read the requested page into the frame.
//    e) Return a pointer to it.
//
//
// If the buffer is full, replace an unpinned page.
//
DiskBlock* BufferMgr::Get( PageId pageId )
{
    auto pred = [ pageId ]( const Frame& f ){ return f.GetPageId() == pageId; };
    auto it = std::find_if( m_pool.begin(), m_pool.end(), pred );

    // The page is already in the buffer
    if( it != m_pool.end() )
    {
        return it->GetBlock();
    }

    return GetFromDisk( pageId );

}

//
// Reads the page from the disk and stores it in the buffer
//
DiskBlock* BufferMgr::GetFromDisk( PageId pageId )
{
    auto pred = []( const Frame& f ){ return f.IsPinned(); };
    auto it = std::find_if_not( m_pool.begin(), m_pool.end(), pred );
    if( it == m_pool.end() )
    {
        throw std::runtime_error( "BufferMgr::GetPageFree: All pages are pinned. The buffer is full." );
    }

    it->Write( m_space );
    it->Read( m_space, pageId );
    return it->GetBlock();
}

//
// Unpin a page so that it can be discarded from the buffer.
//
void BufferMgr::Unpin( PageId pageId , bool dirty )
{
    Frame& frame = FindFrame( pageId );
    if( dirty )
    {
        frame.MarkDirty();
    }
    frame.UnpinPage();
}

//
// Returns a reference to a frame storing the page with the identyfier "pageId".
//
Frame& BufferMgr::FindFrame( PageId pageId )
{
    auto pred = [ pageId ]( const Frame& f ){ return f.GetPageId() == pageId; };
    auto it = std::find_if( m_pool.begin(), m_pool.end(), pred );

    if( it == m_pool.end() )
    {
        throw std::runtime_error( "BufferMgr::FindFrame: Page not in the buffer." );
    }

    return *it;
}


//
//
//
std::pair< PageId, DiskBlock * > BufferMgr::alloc()
{
    const PageId pageId = m_space.Alloc();
    DiskBlock* block = Get( pageId );
    return std::make_pair( pageId, block );
}


//
// Mark a page dirty so that when it is discarded from the buffer
// it will be written back to the file.
//
void BufferMgr::MarkDirty( PageId pageId )
{
    Frame& frame = FindFrame( pageId );
    frame.MarkDirty();
}

//
// Flush all pages hold in the buffer into disk.
//
void BufferMgr::Flush( )
{
    for( Frame& f : m_pool )
    {
        if( f.IsPinned() )
        {
            throw std::runtime_error( "BufferMgr::FlushPages. There are pinned pages." );
        }
        f.Write( m_space );
    }
}
