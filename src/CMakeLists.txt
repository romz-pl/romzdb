cmake_minimum_required (VERSION 3.0)
project (ROMZDB)
add_executable(romzdb.x
buffermgr.cpp
db.cpp
dir.cpp
dirpage.cpp
dirslot.cpp
diskblock.cpp
diskspacemgr.cpp
frame.cpp
heapfile.cpp
heappage.cpp
main.cpp
page.cpp
pageid.cpp
pageoffset.cpp
record.cpp
recordid.cpp
scaner.cpp
slot.cpp
slotid.cpp
unixfile.cpp
)

