
set( LIB_NAME romzdb-util )

add_library( ${LIB_NAME} STATIC
    bitarray.cpp
    div_ceil.cpp
    temp_path.cpp
)

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZDB_CXX_FLAGS} )
