
set( TEST_NAME test-hf )

add_executable( ${TEST_NAME}
    dirpage_test.cpp
    dir_test.cpp
    heapfile_test.cpp
    heappage_test.cpp
    main.cpp
    scaner_test.cpp
)

target_compile_options( ${TEST_NAME} PRIVATE 
    -Wall -pedantic 
    -pthread 
    -g -O0 
    -fprofile-arcs -ftest-coverage 
    -std=c++11 -DNDEBUG )

target_include_directories( ${TEST_NAME} PRIVATE 
    ${gtest_SOURCE_DIR}/include 
    ${gtest_SOURCE_DIR} 
)

target_include_directories( ${TEST_NAME} PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/disk 
    ${PROJECT_SOURCE_DIR}/src/buffer 
    ${PROJECT_SOURCE_DIR}/src/hf
)

target_link_libraries( ${TEST_NAME} 
    -fprofile-arcs 
    gtest 
    gtest_main 
    pthread 
    romzdb-disk 
    romzdb-buffer 
    romzdb-hf
)

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )


