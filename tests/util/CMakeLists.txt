
set( TEST_NAME test-util )

add_executable( ${TEST_NAME}
    bimap.cpp
    bitarray.cpp
    div_ceil.cpp
    main.cpp
)

target_compile_options( ${TEST_NAME} PRIVATE ${ROMZDB_CXX_FLAGS} )

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


