
set( LIB_NAME romzdb-disk ) 

add_library( ${LIB_NAME} STATIC
    bitarray.cpp
    blockid.cpp
    dbfile.cpp
    dbfileid.cpp
    diskblock.cpp
    div_ceil.cpp
    pageid.cpp
    space.cpp
    spacemap.cpp
    temp_path.cpp
    unixfile.cpp
)

target_compile_options( ${LIB_NAME} PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

