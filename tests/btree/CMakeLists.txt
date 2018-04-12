
set( TEST_NAME test-btree )

add_executable( ${TEST_NAME}
    main.cpp
    root_page.cpp
)

target_compile_options( ${TEST_NAME} PRIVATE ${ROMZDB_CXX_FLAGS} )

target_include_directories( ${TEST_NAME} PRIVATE 
    ${gtest_SOURCE_DIR}/include 
    ${gtest_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries( ${TEST_NAME} 
    -fprofile-arcs 
    gtest 
    gtest_main 
    pthread
   romzdb-btree 
    romzdb-hf 
    romzdb-buffer 
    romzdb-disk
    romzdb-util
)


add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

set_tests_properties ( ${TEST_NAME} PROPERTIES DEPENDS test-hf )

