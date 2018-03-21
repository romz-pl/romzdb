## Disk Space Manager

The lowest level of software in the DBMS architecture manages space on disk. Abstractly, the disk
space manager supports the concept of a page as a unit of data and provides
cmmnands to allocate or deallocate a page and read or write a page. The size
of a page is chosen to be the size of a disk block and pages are stored as disk
blocks so that reading or writing a page can be done in one disk I/O.

The disk space manager keeps track of which disk blocks are in use, in addition
to keeping track of which pages are on which disk blocks. Although it is likely
that blocks are initially allocated sequentially on disk, subsequent allocations
and deallocations could in general create 'holes'.

To keep track of block usage we maintain a bitmap with one bit for each disk block, which
indicates whether a block is in use or not. A bitmap also allows very fast
identification and allocation of contiguous areas on disk.


Reference:
Book: "Database Management Systems" by Raghu Ramakrishnan and Johannes Gehrke
Third edition, 2003

## Implementation

The disk space manager (DSM) (implemented as part of the DB class) is the component of ROMZDB that takes 
care of the allocation and deallocation of pages within a database. It also performs reads and writes 
of pages to and from disk, and provides a logical file layer within the context of a database 
management system.

A ROMZDB database is implemented as a set of Unix file. Its pages are simply page-sized blocks of bytes 
within this file. The higher-level structures of a ROMZDB database, such as heap files and B+ tree 
files, are actually logical files consisting of collections of database pages. In discussing 
the Disk Space Manage, whenever necessary we will refer to page-sized blocks of bytes in 
the underlying Unix file as *disk bloks*, and pages in higher-level structures such 
as heap files as, for example, *heap file pages* in order to avoid confusion. 

The DB class provides the abstraction of a single database stored on disk. 
It shields the rest of the software from the fact that the database is implemented 
as a set of Unix files. It provides methods for allocating additional pages (from the 
underlying Unix file) for use in the database and deallocating pages (which may then be 
re-used in response to subsequent allocation requests). (The DB class actually supports allocation 
and deallocation of a consecutive run of pages, though the higher-level code 
usually just asks for one page at a time.) 

The DB class keeps track of allocated space within the database using a fixed set 
of pages called the space map. They can be thought of as containing a bit map: one bit per 
database page, with *0* denoting that the corresponding page is free 
and *1* denoting that it is allocated.

The DB class maintains the space map, in addition to its other duties, updating it 
whenever pages are allocated or deallocated. 

The current implementation creates fixed-size databases; the space map is set when the database 
is created, and never grows. This limitation would be fairly easy to remedy, either by setting 
a maximum database size (still fixing the size of the space map, but allowing the database to grow 
to fit the maximum number of pages representable in the map), or by having the space map be 
a linked list of pages and grow as needed. 

Reference:
Minibase: http://pages.cs.wisc.edu/~dbbook/openAccess/Minibase/spaceMgr/dsm.html


## Author

Name: Zbigniew Romanowski

email: romz@wp.pl


