cmake_minimum_required(VERSION 3.6)

find_package(Boost 1.62.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(clang_on_linux false)
if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_definitions(-std=c++1z -stdlib=libc++ -g -Wall)
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        set(clang_on_linux true)
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    add_definitions(-std=c++1z -g -Wall)
else ()
    message(FATAL_ERROR "Only Clang or GCC with -std=c++1z will work")
endif ()

add_subdirectory(googletest-release-1.8.0)

add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(perf)
add_subdirectory(doc) # Doesn't build docs, just the snippets files.
