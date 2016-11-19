cmake_minimum_required(VERSION 3.6)

add_definitions(-std=c++1z -stdlib=libc++ -g)

add_subdirectory(googletest-release-1.8.0)

find_package(Boost 1.62.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    message(FATAL_ERROR "Only Clang with -std=c++1z will work")
endif()

add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(perf)
