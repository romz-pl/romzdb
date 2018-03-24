
# Buffer Manager

This library implements buffer manager

## Principles

1. A database buffer has to be maintained for purposes of interfacing main memory and disk.

2. In order to facilitate the exchange of data between disk and main storage,
the database is divided into pages of equal size.

3. The number of frames in the buffer can be selected as a DBMS parameter,
which remains constant during a DBMS session.

4. Since a physical access to a database page on disk is much more expensive
than an access to a database page in the buffer, the main goal of a database
buffer manager is the minimization of physical I/O for a given buffer size.

5. A request for a data object is performed as follows:

   * The requesters have to be aware of the page boundaries and must use the
   DBMS catalog, index structures, and so on, to find the page numbers of pages
   they have to access.

   * A page request $P_i$ is issued by the FIX operator (Pin operator) qualified by an
   optional update intent. As a result, $P_i$ is located and fixed (pinned) in the buffer to
   prevent replacement during its use.

   * The address of the frame containing $P_i$ is then passed back.

   * Requestors can now execute machine instructions addressing data objects within $P_i$.
   Since only the requestors know when the addressing phase within $P_i$ ends,
   they must call the buffer manager to again perform the UNFIX operation and
   make $P_i$ eligible for replacement.

6. Each request for a page is called a logical reference. For each logical reference
the buffer manager has to perform the following actions:

   * The buffer is searched and the page is located.
  
   * If the page is not in the buffer, a buffer allocation strategy is used to
   determine the set of candidate buffer pages from which a “victim” for the
   requested page is to be taken. A page replacement algorithm then decides
   which of the buffer pages has to be replaced. Whenever the page selected for
   replacement has been modified, it has to be written back to disk before the
   new page is read into the buffer frame. Each access to a database page on
   disk is called a physical reference, and is one of the most expensive operations
   within a DBMS.

   * The requested page is fixed in the buffer by marking its buffer control block.

   * The fact that the page has been referenced is recorded, since most replacement
   algorithms are based on the history of references to the buffer pages (e.g., LRU).

   * Finally, the address of the buffer frame containing the requested page is
   passed to the calling DBMS component as a return parameter.

7. Whenever a logical reference to a database page occurs, the buffer manager has
to search the buffer. Since this is a frequent event, the search strategy implemented
must be efficient. This is implemented as hash table.

## Characteristic

The buffer manager provides:
1. Sharing. Pages  are  made  addressable  in  the  buffer
pool,  which  is  an  area  of  shared  virtual  memory
accessible to all processes that run the database code.


2. Addressability. Each  access  module  is  returned  an
address  in  the  buffer  pool,  denoting  the  beginning  of
the frame containing the requested page.

3. Semaphore  protection. Many  processes  can  request
accesses to the same page at the same time; the buffer
manager  gives  them  the  same  frame  address.  The
synchronization  of  these  parallel  accesses  in  order  to
preserve   serializability   is   not   the   buffer   manager
problem. It has only to provide a semaphore per page
that   can   be   used   for   implementing   e.g.   locking
protocol.

4. Durable  storage. The  access  modules  inform  the
buffer  manager  if  their  page  access  has  modified  the
page,  however,  the  page  is  written  out  to  disk  by  the
buffer  manager,  probably,  at  a  time  when  update
transaction is already committed.


## Access to pages

All modules operating at the buffer interface must
strictly  follow  the  FIX-USE-UNFIX  protocol, with  the
additional requirements of keeping the duration of a fix
as  short as possible (even if a  module knows that it
might need access to a page again later on).

* FIX. The  client  requests  access  to  a  page  using  the
bufferfix  interface.  The  page  is  fixed  in  the  buffer,
that is, it is not eligible for replacement.

* USE. The client uses the page with the guarantee that
the  pointer  to  the  frame  containing  the  page  will
remain valid.

* UNFIX. The client explicitly waives further usage of the
frame pointer, that is, it tells the buffer manager that
it  no  longer  wants  to  use  that  page.  The  buffer
manager can therefore unfix the page, which means
that the page is eligible for replacement.


## Replacement algorithms for the database buffer

1. The goal of each replacement algorithm is the minimization of the buffer fault
rate for a given buffer size and allocation.

2. If a logical reference to the buffer fails, a page in the buffer must be selected for
replacement to make room for the requested page. Although a comparable
problem arises in OS virtual memory management, there are some important
differences:

   * Any virtual memory page can be replaced at any time, because every reference
   is done by address translation hardware. However, in order to guarantee their
   addressability, some database pages can be fixed in the database buffer and are
   not eligible for replacement.

   * A FIX-UNFIX interval of a database page, in which the calling system
   component issues addressing operations to data objects within that page, is
   treated as one database page reference. However, in OS memory management,
   every machine instruction touching the page is a page reference, resulting in
   substantially different replacement decisions.

3. Replacement algorithms can be classified into prepaging algorithms and demand paging algorithms.

   * Prepaging algorithms fetch not only the requested page, but also m
   additional pages that are physically close to the requested page, thus saving
   considerable access time, compared to (m + 1) individual accesses.

   * Demand paging algorithms fetch only the requested page when there is a page fault.

4. Replacement algorithms replace the buffer page having the lowest probability of
rereference. They usually rely on the characteristics of the past reference string
in order to extrapolate future reference behavior. Their general assumption is
that there is locality of reference; that is, recent reference behavior is a good
indicator for the near future. Hence, the age and the references of a buffer page
can be applied as suitable criteria to predict future reference behavior. By using
logical references as units of time, the age of a page can be measured in an
appropriate way.

   * The algorithm FIFO (first-in, first-out) replaces the oldest buffer page. Independent
   of its reference frequency, the age of a page since the first reference is
   the only decision criterion. Hence, FIFO is only appropriate for sequential access
   behavior.

   * The algorithm LFU (least frequently used) replaces the buffer page with the
   lowest reference frequency. Reference counters (RC) are needed to record all references
   to a buffer page. When a page is fetched, the corresponding RC is initialized to
   1; every rereference increments it by 1. When replacement is necessary, the
   buffer page with the smallest value of RC is chosen; a tie is resolved by some
   mechanism. In this strict LFU realization, the age of a page is not taken into
   account at all; pages with very high reference activity during a short interval can
   obtain such high RC values that they will never be displaced, even if they are
   never referenced again. For this reason, the pure LFU mechanism should not be
   implemented in a database environment.

   * The algorithm LRU (least recently used ) replaces the buffer page that was
   least recently used, and can be explained easily by means of a so-called LRU stack.
   The replacement decision is determined by which page is referenced and by
   the age of each buffer page since its most recent reference. The FIX mechanism
   for pages causes LRU to be optionally implemented by two versions, depending
   on how the term “used” is interpreted, as
      * least recently referenced, or
      * least recently unfixed.
   The version considering the UNFIX time is preferable in DBMS buffer management
   because FIX phases can last a very long time due to delays caused by a
   transaction’s blocking times and action interrupts. Thus, only this (UNFIX time)
   version guarantees the intended observation of the basic LRU idea.

   * The CLOCK algorithm attempts to simulate LRU behavior by means of a
   simpler implementation. CLOCK is a modification of the FIFO mechanism.
   A use-bit is added to every buffer page,
   indicating whether or not the page was referenced during the recent circulation
   of the selection pointer. The page to be replaced is determined by the stepwise
   examination of the use-bits. Encountering a l-bit causes a reset to 0 and the
   move of the selection pointer to the next page. The first page found with a O-bit
   is the victim for replacement.

   * In combining the idea of LFU with the implementation of CLOCK, the basic
   version of GCLOCK (generalized CLOCK) arises. The use-bit of a buffer
   page Pi is replaced by a reference counter (RC). References to Pi increment the
   corresponding counter RC(i). In the basic GCLOCK version, RC(i) is initialized
   to 1 upon first fetch of Pi and incremented by 1 at each rereference of Pi.
   When a buffer fault occurs, a circular search is initiated, decrementing
   stepwise the reference counters until the first with a value of 0 is found.
   This method accomplishes an essential improvement compared to a pure LFU
   algorithm. Nevertheless, this basic GCLOCK version tends to replace the youngest
   buffer pages, independent of their type and actual probability of rereference.
   To improve this undesired behavior, a number of variations were introduced.

## References

1. W. EFFELSBERG, T. HAERDER, "Principles of Database Buffer Management"
ACM Transactions on Database Systems, Vol. 9, No. 4, December 1984, Pages 560-595.

2. R. Ramakrishnan, J. Gehrke "Database Management Systems" Third edition, 2003

