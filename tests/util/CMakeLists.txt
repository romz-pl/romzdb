
set( TEST_NAME test-util )

add_executable( ${TEST_NAME}
    bitarray.cpp
    div_ceil.cpp
    temp_path.cpp
    main.cpp
)

target_compile_options( ${TEST_NAME} PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++17 )

target_include_directories( ${TEST_NAME} PRIVATE 
    ${gtest_SOURCE_DIR}/include 
    ${gtest_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/src )

target_link_libraries( ${TEST_NAME} 
    -fprofile-arcs 
    gtest 
    gtest_main 
    pthread 
    romzdb-util )

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )


