
set( TEST_NAME test-disk )

add_executable( ${TEST_NAME}
    bitarray.cpp
    dbfile.cpp
    main.cpp
    space.cpp
    unixfile.cpp
)

target_compile_options( ${TEST_NAME} PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 )

target_include_directories( ${TEST_NAME} PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} )

target_include_directories( ${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/disk )

target_link_libraries( ${TEST_NAME} -fprofile-arcs gtest gtest_main pthread romzdb-disk )

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )


