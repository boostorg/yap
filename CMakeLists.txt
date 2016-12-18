cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(dependencies)

add_library(yap INTERFACE)
target_include_directories(yap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(yap INTERFACE boost)


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

add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(perf)
add_subdirectory(doc) # Doesn't build docs, just the snippets files.
