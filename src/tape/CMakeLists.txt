set(SOURCES_FILE tape-file.cpp)
set(HEADERS_FILE tape-file.hpp)
add_library(tape-file ${SOURCES_FILE} ${HEADERS_FILE})
target_include_directories(tape-file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES_SORT sort.cpp)
set(HEADERS_SORT sort.hpp)
add_library(tape-sort-algo ${SOURCES_SORT} ${HEADERS_SORT})
target_include_directories(tape-sort-algo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tape-file tape-sort-algo)
