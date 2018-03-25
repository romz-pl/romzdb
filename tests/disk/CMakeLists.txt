
set( TEST_NAME test-disk )

add_executable( ${TEST_NAME}
    blockid.cpp
    dbfile.cpp
    dbfileid.cpp
    main.cpp
    space.cpp
    unixfile.cpp
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
    romzdb-disk 
    romzdb-util 
)

add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

set_tests_properties ( ${TEST_NAME} PROPERTIES DEPENDS test-util )

