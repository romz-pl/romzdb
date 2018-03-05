# romzdb: a framework for heap file database

[![Build Status](https://travis-ci.org/romz-pl/romzdb.svg?branch=master)](https://travis-ci.org/romz-pl/romzdb)
[![Coverage Status](https://codecov.io/gh/romz-pl/romzdb/badge.svg?branch=master)](https://codecov.io/gh/romz-pl/romzdb?branch=master)

## Overview

1. This is an implementation of heap file storing the variable length records.

2. The record length must fit on one disk page. 

3. The size of the page is fixed during the compilation. The current value is 4096 bytes.

4. All data are stored in one file. The file is implemented in class UnixFile.

5. The data to the UnixFile is transferred in pages of fixed size. This is managed by class DiskSpaceMgr.

6. Buffer manager (implemented in class BufferMgr) is responsible for transferring data from disk into memory and transferring data from memory to disk. Disk blocks in the buffer manager are kept in frames (class Frame), where "dirty" bit and "pin-count" are stored.

7. Heap file is a sequence of pages. It uses buffer manager for page management. 

8. Pages in heap file are managed by directory of pages. 

9. On the page of heap file variable length record can be stored. 

10. The program is implemented in C++.

11. There is an extensive set of tests written in Google test format.

## Literature

The project romzdb is based on: 
1. RedBase project from Stanford University
   See https://web.stanford.edu/class/cs346/2015/redbase.html

2. Book: "Database Management Systems" by Raghu Ramakrishnan and Johannes Gehrke
   http://pages.cs.wisc.edu/~dbbook/
   and associated MiniBase project with tis book
   http://pages.cs.wisc.edu/~dbbook/openAccess/Minibase/minibase.html

3. Book: "Fundamentals of Database Systems" by Ramez Elmasri and Shamkant B. Navathe

4. Book: "Database System Concepts" by Abraham Silberschatz, Henry Korth, S. Sudarshan 

5. Paper: "Principles of Database Buffer Management" by WOLFGANG EFFELSBERG and THEO HAERDER
   ACM Transactions on Database Systems, Vol. 9, No. 4, December 1984, Pages 560-595.

## Compilation

In order to compile, and run all tests execute bash script: `./build-all.sh`

## Author

Name: Zbigniew Romanowski

email: romz@wp.pl


