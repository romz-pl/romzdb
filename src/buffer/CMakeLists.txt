


add_library(romzdb-buffer STATIC
buffermgr.cpp
frame.cpp
page.cpp
#pageoffset.cpp
)

target_include_directories( romzdb-buffer PRIVATE ../disk )
target_compile_options( romzdb-buffer PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

