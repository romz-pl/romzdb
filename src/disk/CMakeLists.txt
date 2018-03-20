
set( LIB_NAME romzdb-disk ) 

add_library( ${LIB_NAME} STATIC
    disk.cpp
    diskblock.cpp
    diskspacemgr.cpp
    pageid.cpp
    unixfile.cpp
)

target_compile_options( ${LIB_NAME} PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

