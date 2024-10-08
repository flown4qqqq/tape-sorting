set(SOURCES foo.cpp)
set(HEADERS foo.hpp)
add_library(foo ${SOURCES} ${HEADERS})
target_include_directories(foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
