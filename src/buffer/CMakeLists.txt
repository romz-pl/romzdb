
set( LIB_NAME romzdb-buffer )

add_library( ${LIB_NAME} STATIC
    buffclock.cpp
    buffermgr.cpp
    frame.cpp
    frameclock.cpp
    frameid.cpp
    page.cpp
)


target_include_directories( ${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src )

target_compile_options( ${LIB_NAME} PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 )

