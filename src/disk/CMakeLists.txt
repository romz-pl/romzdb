


add_library(romzdb-disk STATIC
diskblock.cpp
diskspacemgr.cpp
pageid.cpp
unixfile.cpp
)

target_compile_options( romzdb-disk PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

