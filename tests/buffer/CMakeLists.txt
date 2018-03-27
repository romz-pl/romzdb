
set( TEST_NAME test-buffer )

add_executable( ${TEST_NAME}
    buffermgr.cpp
    frame.cpp
    main.cpp
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
    romzdb-disk 
    romzdb-buffer
    romzdb-util
)


add_test( NAME ${TEST_NAME} COMMAND ${TEST_NAME} )

set_tests_properties ( ${TEST_NAME} PROPERTIES DEPENDS test-disk )


