#include "buffermgr.h"
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <algorithm>

//
// Constructor
// numPage - number of pages in the buffer manager.
//
BufferMgr::BufferMgr( DiskSpaceMgr& ds, std::size_t numPages )
    : m_ds( ds )
    , m_pool( numPages )
{

}

//
//
//
BufferMgr::~BufferMgr()
{
    FlushPages();
}

//
// Returns  a pointer to a page pinned in the buffer.
//
//
// 1. Check the buffer pool to see if it contains the requated page.
//    If the page is already in the buffer:
//        a) (re)pin the page,
//        b) return a pointer to it.
//
//
// 2. When the page is not in the buffer pool, do the following:
//    a) Choose a frame for replacement, using the repacement policy.
//    b) Increment its "pin count"
//    c) If its "dirty bit" is on, write the page it contains to disk
//       (taht is, the disk copy of the page is overwritten with the contents of the frame).
//    d) Read the requested page into the frame.
//    e) Return a pointer to it.
//
//
// If the buffer is full, replace an unpinned page.
// 
// If multiplePins is true, the page can have "pin_count > 1" 
//
Page* BufferMgr::GetPage( PageId pageId, bool multiplePins )
{
    auto pred = [pageId]( const Frame& f ){ return f.IsEqual( pageId ); };
    auto it = std::find_if( m_pool.begin(), m_pool.end(), pred );

    // The page is already in the buffer
    if( it != m_pool.end() )
    {
        Frame& frame = *it;
        // Error if we don't want to get a pinned page
        if( !multiplePins && frame.IsPinned() )
        {
            throw std::runtime_error( "BufferMgr::GetPage. Multiple pins not allowed and the page already pinned." );
        }

        return frame.GetPage();
    }

    return GetPageFromDisk( pageId );

}

//
// Reads the page from the disk and sotes it in the buffer
//
Page* BufferMgr::GetPageFromDisk( PageId pageId )
{
    auto pred = []( const Frame& f ){ return !f.IsPinned(); };
    auto it = std::find_if( m_pool.begin(), m_pool.end(), pred );
    if( it == m_pool.end() )
    {
        throw std::runtime_error( "BufferMgr::GetPageFree: All pages are pinned. The buffer is full." );
    }

    Frame& f = *it;
    f.Write( m_ds );
    f.Read( m_ds, pageId );
    return f.GetPage();
}

//
// Returns the pointer to a frame storing the page with the identyfier "pageId".
//
Frame* BufferMgr::FindFrame( PageId pageId )
{
    auto pred = [pageId]( const Frame& f ){ return f.IsEqual( pageId ); };
    auto it = std::find_if( m_pool.begin(), m_pool.end(), pred );
    if( it != m_pool.end() )
    {
        return &(*it);
    }

    return nullptr;
}



//
// Unpin a page so that it can be discarded from the buffer.
//
void BufferMgr::UnpinPage( PageId pageId )
{
    Frame* frame = FindFrame( pageId );
    if( !frame )
    {
        throw std::runtime_error( "BufferMgr::UnpinPage: Page not in the buffer." );
    }

    frame->UnpinPage();
}

//
// Mark a page dirty so that when it is discarded from the buffer
// it will be written back to the file.
//
void BufferMgr::MarkDirty( PageId pageId )
{
    Frame* frame = FindFrame( pageId );
    if( !frame )
    {
        throw std::runtime_error( "BufferMgr::MarkDirty: Page not in the buffer." );
    }

    frame->MarkDirty();
}

//
// Flush all pages hold in the buffer into disk.
//
void BufferMgr::FlushPages( )
{
    for( auto& f : m_pool )
    {
        if( f.IsPinned() )
        {
            throw std::runtime_error( "BufferMgr::FlushPages. There are pinned pages." );
        }
        f.Write( m_ds );
    }
}

//
// Writes the page from the buffer into disk.
//
void BufferMgr::WritePage( PageId pageId )
{
    Frame* frame = FindFrame( pageId );
    if( !frame )
    {
        throw std::runtime_error( "BufferMgr::WritePage: Page not in the buffer." );
    }

    frame->Write( m_ds );
}

/*
//
// Get a pointer to a page pinned in the buffer. If the page is
// already in the buffer, (re)pin the page and return a pointer
// to it.  If the page is not in the buffer, read it from the file,
// pin it, and return a pointer to it.  If the buffer is full,
// replace an unpinned page.
//
Page* BufferMgr::GetPage( UnixFile* uf, PageId pageNum, bool bMultiplePins )
{
    // buffer slot where page is located
    Frame* frame = m_usedMap.Find( uf, pageNum );

    // Page frame not found
    if( !frame )
    {
        // Allocate an empty page, this will also promote the newly allocated page to the MRU slot
        frame = InternalAlloc( );

        // Read the page, insert it into the hash table, and initialize the page description entry
        frame->ReadPage( uf, pageNum );
        m_usedMap.Insert( uf, pageNum, frame );
    }
    else // Page is in the buffer...
    {
        // Error if we don't want to get a pinned page
        if( !bMultiplePins && frame->m_pinCount > 0 )
        {
            throw std::runtime_error( "BufferMgr::GetPage. Page already pinned." );
        }
        // Page is alredy in memory, just increment pin count
        frame->m_pinCount++;

        // Make "frame" the most recently used page
        m_usedList.remove( frame );
        m_usedList.push_front( frame );
    }

    assert( IsInvariant() );

    return &( frame->m_page );

}

//
// Allocate a new page in the buffer and return a pointer to it.
//
Page* BufferMgr::AllocatePage( UnixFile* uf, PageId pageNum )
{
    Frame* frame = m_usedMap.Find( uf, pageNum );
    if( frame )
    {
        throw std::runtime_error( "BufferMgr::AllocatePage. Page is already in buffer." );
    }

    frame = InternalAlloc();
    frame->Init( uf, pageNum );
    m_usedMap.Insert( uf, pageNum, frame );

    assert( IsInvariant() );

    return &( frame->m_page );
}

//
// Mark a page dirty so that when it is discarded from the buffer
//       it will be written back to the file.
//
void BufferMgr::MarkDirty( UnixFile* uf, PageId pageNum )
{
    Frame* frame = m_usedMap.Find( uf, pageNum );
    assert( frame );
    assert( frame->m_pinCount > 0 );

    frame->m_dirty = true;

    // Make "frame" the most recently used page
    m_usedList.remove( frame );
    m_usedList.push_front( frame );

    assert( IsInvariant() );
}


//
// Unpin a page so that it can be discarded from the buffer.
//
void BufferMgr::UnpinPage( UnixFile* uf, PageId pageNum )
{
    Frame* frame = m_usedMap.Find( uf, pageNum );
    assert( frame );
    assert( frame->m_pinCount > 0 );

    frame->m_pinCount--;

    if( frame->m_pinCount == 0 )
    {
        // Make "frame" the most recently used page
        m_usedList.remove( frame );
        m_usedList.push_front( frame );
    }

    assert( IsInvariant() );

}

//
// Release all pages for this file and put them onto the free list
// Returns a warning if any of the file's pages are pinned.
// The linear search of the buffer is performed.
// A better method is not needed because # of buffers are small.
//
void BufferMgr::FlushPages( UnixFile* uf )
{
    auto it = m_usedList.begin();
    while( it != m_usedList.end() )
    {
        Frame *frame = *it;
        if( frame->m_uf != uf )
        {
            it++;
            continue;
        }

        if( frame->m_pinCount > 0 )
        {
            throw std::runtime_error( "BufferMgr::FlushPages. There are pinned pages." );
        }

        if( frame->m_dirty )
            frame->WritePage();

        m_usedMap.Delete( uf, frame->m_pageNum );
        it = m_usedList.erase( it );
        m_freeList.push_front( frame );

    }

    assert( IsInvariant() );
}

//
// Flushes all pages in the buffer
//
void BufferMgr::FlushPages( )
{
    auto it = m_usedList.begin();
    while( it != m_usedList.end() )
    {
        Frame *frame = *it;
        assert( frame->m_uf );

        if( frame->m_pinCount > 0 )
        {
            throw std::runtime_error( "BufferMgr::FlushPages(*). There are pinned pages." );
        }
        if( frame->m_dirty )
            frame->WritePage();

        m_usedMap.Delete( frame->m_uf, frame->m_pageNum );
        it = m_usedList.erase( it );
        m_freeList.push_front( frame );
    }

    assert( m_usedMap.Size() == 0 );
    assert( m_usedList.size() == 0 );
    assert( m_freeList.size() == m_buffer.size() );
    assert( IsInvariant() );
}

//
// If a page is dirty then force the page from the buffer pool
// onto disk.  The page will not be forced out of the buffer pool.
//
//
void BufferMgr::ForcePages( UnixFile* uf, PageId pageNum )
{
    for( auto it = m_usedList.begin(); it != m_usedList.end(); ++it )
    {
        Frame *frame = *it;

        // If the page belongs to the passed-in file descriptor
        if( frame->m_uf == uf && ( pageNum==ALL_PAGES || frame->m_pageNum == pageNum) )
        {
            // I don't care if the page is pinned or not, just write it ifit is dirty.
            if( frame->m_dirty )
            {
                frame->WritePage();
            }
        }
    }
}

//
// Display all of the pages within the buffer.
// This routine will be called via the system command.
//
void BufferMgr::PrintBuffer()
{
    std::cout << "Buffer contains " << m_buffer.size() << " pages of size " << Page::m_size <<".\n";
    std::cout << "Contents in order from most recently used to least recently used.\n";

    for( auto it = m_usedList.begin(); it != m_usedList.end(); ++it )
    {
        Frame *frame = *it;

        // cout << slot << " :: \n";
        std::cout << "  fd = " << frame->m_uf << "\n";
        std::cout << "  pageNum = " << frame->m_pageNum << "\n";
        std::cout << "  bDirty = " << frame->m_dirty << "\n";
        std::cout << "  pinCount = " << frame->m_pinCount << "\n";
    }

    if( m_usedList.empty() )
    {
        std::cout << "Buffer is empty!\n";
    }
    else
    {
        std::cout << "All remaining slots are free.\n";
    }
}


//
// Remove all entries from the buffer manager.
// This routine will be called via the system command and is only
// really useful if the user wants to run some performance
// comparison starting with an clean buffer.
//
void BufferMgr::ClearBuffer()
{
    auto it = m_usedList.begin();
    while( it != m_usedList.end() )
    {
        Frame *frame = *it;

        if( frame->m_pinCount == 0 )
        {
            m_usedMap.Delete( frame->m_uf, frame->m_pageNum );
            it = m_usedList.erase( it );
            m_freeList.push_front( frame );
        }
        else
        {
            ++it;
        }
    }

    assert( IsInvariant() );
}


//
// Resizes the buffer manager to the size passed in.
// This routine will be called via the system command.
//
void BufferMgr::ResizeBuffer(int )
{
    assert( 0 );
}

// Allocate a buffer slot. The slot is inserted at the
// head of the used list.  Here's how it chooses which slot to use:
// If there is something on the free list, then use it.
// Otherwise, choose a victim to replace.  If a victim cannot be
// chosen (because all the pages are pinned), then return an error.
//
Frame* BufferMgr::InternalAlloc( )
{
    Frame* frame = nullptr;
    // If the free list is not empty, choose a slot from the free list
    if( !m_freeList.empty() )
    {
        frame = m_freeList.front();
        m_freeList.pop_front();
    }
    else
    {
        // Choose the least-recently used page that is unpinned
        for( auto it = m_usedList.rbegin() ; it != m_usedList.rend(); ++it )
        {
            if( (*it)->m_pinCount == 0 )
            {
                frame = *it;
                // m_usedList.erase( it ); // This does not work for reverse iterators
                m_usedList.remove( frame );
                break;
            }
        }

        // Return error if all buffers were pinned
        if( !frame )
        {
            throw std::runtime_error( "BufferMgr::InternalAlloc. All buffers were pinned" );
        }

        // Write out the page if it is dirty
        if ( frame->m_dirty )
        {
           frame->WritePage();
        }

        // Remove page from the hash table and slot from the used buffer list
        m_usedMap.Delete( frame->m_uf, frame->m_pageNum );

    }

    // Link slot at the head of the used list
    m_usedList.push_front( frame );

    return frame;
}

//
// Returns true, if invariant is satisfied.
//
bool BufferMgr::IsInvariant()
{
    const bool b1 = ( m_usedMap.Size() == m_usedList.size() );
    const bool b2 = ( m_usedList.size() + m_freeList.size() == m_buffer.size() );

    return b1 && b2;
}

*/

