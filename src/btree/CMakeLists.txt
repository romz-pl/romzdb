
set( LIB_NAME romzdb-btree )

add_library( ${LIB_NAME} STATIC
    root_page.cpp
)

target_include_directories( ${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src )

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZDB_CXX_FLAGS} )


