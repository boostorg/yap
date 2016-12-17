include_directories(${CMAKE_HOME_DIRECTORY})

macro(add_sample name)
    add_executable(${name} ${name}.cpp)
    if (clang_on_linux)
        target_link_libraries(${name} c++)
    endif ()
endmacro()

add_sample(minimal)
add_sample(hello_world)
add_sample(hello_world_redux)
add_sample(calc1)
add_sample(calc2a)
add_sample(calc2b)
add_sample(calc3)
add_sample(lazy_vector)
add_sample(tarray)
add_sample(vec3)
add_sample(vector)
add_sample(mixed)
add_sample(map_assign)
add_sample(future_group)

# See https://github.com/fqiang/autodiff_library
include_directories(autodiff_library)
add_executable(
    autodiff
    autodiff_example.cpp
    autodiff_library/ActNode.cpp
    autodiff_library/BinaryOPNode.cpp
    autodiff_library/Edge.cpp
    autodiff_library/EdgeSet.cpp
    autodiff_library/Node.cpp
    autodiff_library/OPNode.cpp
    autodiff_library/PNode.cpp
    autodiff_library/Stack.cpp
    autodiff_library/Tape.cpp
    autodiff_library/UaryOPNode.cpp
    autodiff_library/VNode.cpp
    autodiff_library/autodiff.cpp
)
if (clang_on_linux)
    target_link_libraries(autodiff c++)
endif ()
