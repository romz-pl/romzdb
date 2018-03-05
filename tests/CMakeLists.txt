

add_executable(tests
buffermgr_test.cpp
db_test.cpp
dir_test.cpp
dirpage_test.cpp
diskspacemgr_test.cpp
heapfile_test.cpp
heappage_test.cpp
main.cpp
scaner_test.cpp
unixfile_test.cpp
)

target_compile_options( tests PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

target_include_directories( tests PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} )

target_include_directories( tests PUBLIC ${PROJECT_SOURCE_DIR}/src )

target_link_libraries( tests -fprofile-arcs gtest gtest_main pthread romzdb )

add_test(NAME romzdb_test COMMAND tests)


#
#target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/googletest/include ${PROJECT_SOURCE_DIR}/src)
#
#
#find_library(GG_LIB gtest.a HINT ${PROJECT_SOURCE_DIR}/3rdparty/googletest)
#target_link_libraries(tests ${GG_LIB} pthread lib-romzdb)
#
