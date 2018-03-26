
set( LIB_NAME romzdb-hf )

add_library( ${LIB_NAME} STATIC
    #    db.cpp
    #dir.cpp
    # dirpage.cpp
    # dirslot.cpp
    #heapfile.cpp
    #heappage.cpp
    #pageoffset.cpp
    record.cpp
    recordid.cpp
    #scaner.cpp
    #slot.cpp
    slotid.cpp
)

target_include_directories( ${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src )

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZDB_CXX_FLAGS} )


