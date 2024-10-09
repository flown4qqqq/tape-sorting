set(SOURCES tape-file.cpp)
set(HEADERS tape-file.hpp)
add_library(tape-file ${SOURCES} ${HEADERS})
target_include_directories(tape-file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
