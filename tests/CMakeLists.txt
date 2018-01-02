

add_executable(test.x
buffermgr_test.cpp
db_test.cpp
dir_test.cpp
dirpage_test.cpp
diskspacemgr_test.cpp
heapfile_test.cpp
heappage_test.cpp
main.cpp
scaner_test.cpp
unixfile_test.cpp
)

target_include_directories(test.x PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/googletest/include ${PROJECT_SOURCE_DIR}/src)


find_library(GG_LIB gtest.a HINT ${PROJECT_SOURCE_DIR}/3rdparty/googletest)
target_link_libraries(test.x ${GG_LIB} pthread lib-romzdb)