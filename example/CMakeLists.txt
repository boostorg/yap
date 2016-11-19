include_directories(${CMAKE_HOME_DIRECTORY})

macro(add_sample name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} c++)
endmacro()

add_sample(hello_world)
