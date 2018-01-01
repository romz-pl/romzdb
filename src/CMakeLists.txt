


add_library(lib-romzdb STATIC
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

add_executable(romzdb.x main.cpp)
target_link_libraries(romzdb.x lib-romzdb)
