

add_executable(test-disk
diskspacemgr_test.cpp
main.cpp
unixfile_test.cpp
)

target_compile_options( test-disk PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

target_include_directories( test-disk PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} )

target_include_directories( test-disk PUBLIC ${PROJECT_SOURCE_DIR}/src/disk )

target_link_libraries( test-disk -fprofile-arcs gtest gtest_main pthread romzdb-disk )

add_test( NAME test-disk COMMAND test-disk )


#
#target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/googletest/include ${PROJECT_SOURCE_DIR}/src)
#
#
#find_library(GG_LIB gtest.a HINT ${PROJECT_SOURCE_DIR}/3rdparty/googletest)
#target_link_libraries(tests ${GG_LIB} pthread lib-romzdb)
#
