


add_library(romzdb-hf STATIC
dir.cpp
dirpage.cpp
dirslot.cpp
heapfile.cpp
heappage.cpp
pageoffset.cpp
record.cpp
recordid.cpp
slot.cpp
slotid.cpp
)

target_include_directories( romzdb-hf PRIVATE ../disk ../buffer )

target_compile_options( romzdb-hf PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

