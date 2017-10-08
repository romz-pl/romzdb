#ifndef ROMZDB_BUFFERMGR_H
#define ROMZDB_BUFFERMGR_H



//
// Buffer Manager
//
// See article
// W. EFFELSBERG, T. HAERDER, "Principles of Database Buffer Management"
// ACM Transactions on Database Systems, Vol. 9, No. 4, December 1984, Pages 560-595.
//
// 1. A database buffer has to be maintained for purposes of interfacing main memory and disk.
//
// 2. In order to facilitate the exchange of data between disk and main storage,
//    the database is divided into pages of equal size.
//
// 3. The number of frames in the buffer can be selected as a DBMS parameter,
//    which remains constant during a DBMS session.
//
// 4. Since a physical access to a database page on disk is much more expensive
//    than an access to a database page in the buffer, the main goal of a database
//    buffer manager is the minimization of physical I/O for a given buffer size.
//
// 5. A request for a data object is performed as follows:
//   5a) The requesters have to be aware of the page boundaries and must use the
//       DBMS catalog, index structures, and so on, to find the page numbers of pages
//       they have to access.
//   5b) A page request $P_i$ is issued by the FIX operator (Pin operator) qualified by an
//       optional update intent. As a result, $P_i$ is located and fixed (pinned) in the buffer to
//       prevent replacement during ita use.
//   5c) The address of the frame containing $P_i$ is then passed back.
//   5d) Requestors can now execute machine instructions addressing data objects within $P_i$.
//       Since only the requestors know when the addressing phase within $P_i$ ends,
//       they must call the buffer manager to again perform the UNFIX operation and
//       make $P_i$ eligible for replacement.
//
// 6. Each request for a page is called a logical reference. For each logical reference
//    the buffer manager has to perform the following actions:
//  6a) The buffer is searched and the page is located.
//  6b) If the page is not in the buffer, a buffer allocation strategy is used to
//      determine the set of candidate buffer pages from which a “victim” for the
//      requested page is to be taken. A page replacement algorithm then decides
//      which of the buffer pages has to be replaced. Whenever the page selected for
//      replacement has been modified, it has to be written back to disk before the
//      new page is read into the buffer frame. Each access to a database page on
//      disk is called aphysical reference, and is one of the most expensive operations
//      within a DBMS.
//  6c) The requested page is fixed in the buffer by marking its buffer control block.
//  6d) The fact that the page has been referenced is recorded, since most replacement
//      algorithms are based on the history of references to the buffer pages (e.g., LRU).
//  6d) Finally, the address of the buffer frame containing the requested page is
//      passed to the calling DBMS component as a return parameter.
//
// 7. Whenever a logical reference to a database page occurs, the buffer manager has
//    to search the buffer. Since this is a frequent event, the search strategy implemented
//    must be efficient. This is implemented as hash table.



#include <map>
#include <vector>
#include <queue>
#include "frame.h"
#include "page.h"
#include "diskspacemgr.h"


class BufferMgr
{

public:
    BufferMgr( DiskSpaceMgr& ds, std::size_t numPages );
    ~BufferMgr();


    Page *GetPage( PageId pageId, bool multiplePins );
    void UnpinPage( PageId pageId );
    void MarkDirty( PageId pageId );

    void FlushPages( );

/*    static Page* AllocatePage( UnixFile* uf, PageId pageNum );
    static void MarkDirty( UnixFile* uf, PageId pageNum );
    static void UnpinPage( UnixFile* uf, PageId pageNum );
    static void FlushPages( UnixFile* uf );
    static void FlushPages();
    static void ForcePages( UnixFile* uf, PageId pageNum = ALL_PAGES );

    static void PrintBuffer();
    static void ClearBuffer();
    static void ResizeBuffer( int );

private:
    static Frame *InternalAlloc();
    static bool IsInvariant();
*/
private:
    Page* GetPageUsed( PageId pageId, bool multiplePins );
    Page* GetPageFree( PageId pageId );

    Frame* FindFrame( PageId pageId );

private:
    DiskSpaceMgr& m_ds;

    // Pool of frames stored in the buffer manager
    std::vector< Frame > m_pool;

};

#endif
