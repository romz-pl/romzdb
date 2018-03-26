cmake_minimum_required (VERSION 3.0)
project (romzdb)

SET( ROMZDB_CXX_FLAGS -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++17 )

include(CTest)
enable_testing(true)

add_subdirectory(3rdparty/googletest)

add_subdirectory(src/disk)
add_subdirectory(src/buffer)
add_subdirectory(src/hf)
add_subdirectory(src/util)

add_subdirectory(tests/disk)
add_subdirectory(tests/buffer)
add_subdirectory(tests/hf)
add_subdirectory(tests/util)
