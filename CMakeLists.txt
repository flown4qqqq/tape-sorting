#################################################################################
##
##                        Information about project
##
#################################################################################
cmake_minimum_required(VERSION 3.22.1)
project(tape-sorting LANGUAGES CXX)

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

#################################################################################
##
##                         Project's properties
##
#################################################################################

option(MYLIB_TESTING "Включить модульное тестирование" ON)

#################################################################################
##
##                       Compilation's properties
##
#################################################################################

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)

add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
    -Wzero-as-null-pointer-constant
)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

#################################################################################
##
##                         Get executable file
##
#################################################################################

add_executable(tape-sort src/main.cpp)

#################################################################################
##
##                           Compiling sources
##
#################################################################################

add_subdirectory(src)
target_link_libraries(tape-sort tape-file)
target_link_libraries(tape-sort cli)

#################################################################################
##
##                              Testing
##
#################################################################################

if(NOT MYLIB_TESTING)
    message(STATUS "Тестирование проекта tape-sorting выключено")
else()
    enable_testing()
    add_subdirectory(ut)
endif()
