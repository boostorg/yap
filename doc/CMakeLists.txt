include_directories(${CMAKE_HOME_DIRECTORY})

macro(add_snippets name)
    add_executable(${name} ${name}.cpp)
    if (clang_on_linux)
        target_link_libraries(${name} c++)
    endif ()
endmacro()

add_snippets(user_macros_snippets)
