# romzdb

[![Build Status](https://travis-ci.org/romz-pl/romzdb.svg?branch=master)](https://travis-ci.org/romz-pl/romzdb)

1. This is an implementation of heap file storing the variable length records.

2. The program is implemented in C++.

3. All data are stored in one file. The file is implemented in class UnixFile.

4. The data to the UnixFile is transferred in pages of fixed size. This is managed by class DiskSpaceMgr.

5. Buffer manager (implemented in class BufferMgr) is responsible for transferring data from disk into memory and transferring data from memory to disk. Disk blocks in the buffer manager are kept in frames (class Frame), where "dirty" bit and "pin-count" are stored.

6. Heap file is a sequence of pages. It uses buffer manager for page management. 

7. Pages in heap file are managed by directory of pages. 

8. On the page of heap file variable length record can be stored. 

9. The project romzdb is based on: 

a. RedBase project from Stanford University
   See https://web.stanford.edu/class/cs346/2015/redbase.html

b. Book: "Database Management Systems" by Raghu Ramakrishnan and Johannes Gehrke
   http://pages.cs.wisc.edu/~dbbook/
   and associated MiniBase project with tis book
   http://pages.cs.wisc.edu/~dbbook/openAccess/Minibase/minibase.html

c. Book: "Fundamentals of Database Systems" by Ramez Elmasri and Shamkant B. Navathe

d. Book: "Database System Concepts" by Abraham Silberschatz, Henry Korth, S. Sudarshan 

e. Paper: "Principles of Database Buffer Management" by WOLFGANG EFFELSBERG and THEO HAERDER
   ACM Transactions on Database Systems, Vol. 9, No. 4, December 1984, Pages 560-595.


Author: Zbigniew Romanowski

emil: romz@wp.pl

