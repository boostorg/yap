cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Boost 1.62.0 EXACT)
if (Boost_INCLUDE_DIRS)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message("-- Boost was not found; attempting to download it if we haven't already...")
    set(boost_url https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.bz2)
    include(ExternalProject)
    ExternalProject_Add(
        Boost
        PREFIX ${CMAKE_BINARY_DIR}/boost_1_62_0
        URL ${boost_url}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )
    include_directories(${CMAKE_BINARY_DIR}/boost_1_62_0/src/Boost)
endif ()

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
add_subdirectory(benchmark-v1.1.0)

add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(perf)
add_subdirectory(doc) # Doesn't build docs, just the snippets files.
