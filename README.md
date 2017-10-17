# romzdb

[![Build Status](https://travis-ci.org/romz-pl/romzdb.svg?branch=master)](https://travis-ci.org/romz-pl/romzdb)

1. This is an implemenation of feap file storing the varaible length records.

2. The project romzdb is based on: 

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


3. The program is implemented in C++.

4. The structure of the program is as follows:

a. All data are stored in one file. The file is implemented in class UnixFile.

b. The data to the UnixFile is tranfered in pages of fixed size. This is managed by class DiskSpaceMgr.

c. Buffer magager (implemented in class BufferMgr) is responsible for tranfering data from disk into memory and transfering data from memory to disk. Disk blocks in the buffer manager are kept in frames (class Frame), where "dirty" bit and "pin-count" are stored.

d. Heap file is a sequence of pagees. It uses buffer manager for page management. Pages in heap file are managed by directory of pages. On the page of heap file variable length record can be stored. 

Author: Zbigniew Romanowski

emil: romz@wp.pl

