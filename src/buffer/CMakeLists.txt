
set( LIB_NAME romzdb-buffer )

add_library( ${LIB_NAME} STATIC
    buffermgr.cpp
    frame.cpp
    page.cpp
)


target_include_directories( ${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src )

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZDB_CXX_FLAGS} )


