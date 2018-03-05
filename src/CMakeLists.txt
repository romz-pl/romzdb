


add_library(romzdb STATIC
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

target_compile_options( romzdb PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

#add_executable(romzdb.x main.cpp)
#target_link_libraries(romzdb.x lib-romzdb)
