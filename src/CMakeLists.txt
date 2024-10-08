#################################################################################
##
##                        Information about project
##
#################################################################################
cmake_minimum_required(VERSION 3.22.1)

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

#################################################################################
##
##                           Linking libraries
##
#################################################################################

set(SOURCES foo.cpp)
set(HEADERS foo.hpp)
add_library(foo ${SOURCES} ${HEADERS})
target_include_directories(foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
