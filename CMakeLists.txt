cmake_minimum_required (VERSION 3.0)
project (ROMZDB)

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

add_subdirectory(src)
add_subdirectory(tests)
