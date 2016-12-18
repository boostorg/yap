include_directories(${CMAKE_HOME_DIRECTORY})
include_directories(${CMAKE_HOME_DIRECTORY}/benchmark-v1.1.0/include)


macro(add_code_gen_executable name)
    add_executable(${name} ${name}.cpp)
    if (clang_on_linux)
        target_link_libraries(${name} c++)
    endif ()
endmacro()

add_code_gen_executable(code_gen_samples)
add_code_gen_executable(map_assign_code_gen)


macro(add_perf_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} benchmark)
    if (clang_on_linux)
        target_link_libraries(${name} c++)
    endif ()
endmacro()

add_perf_executable(map_assign_perf)
add_perf_executable(arithmetic_perf)

include(Disassemble)
foreach(fun eval_as_cpp_expr eval_as_yap_expr eval_as_cpp_expr_4x eval_as_yap_expr_4x)
    disassemble(disassemble.arithmetic_perf.${fun} EXECUTABLE arithmetic_perf FUNCTION ${fun})
    disassemble(disassemble.code_gen_samples.${fun} EXECUTABLE code_gen_samples FUNCTION ${fun})
endforeach()

add_custom_target(perf
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.arithmetic_perf.eval_as_cpp_expr
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.arithmetic_perf.eval_as_yap_expr
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.arithmetic_perf.eval_as_cpp_expr_4x
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.arithmetic_perf.eval_as_yap_expr_4x

    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.code_gen_samples.eval_as_cpp_expr
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.code_gen_samples.eval_as_yap_expr
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.code_gen_samples.eval_as_cpp_expr_4x
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target disassemble.code_gen_samples.eval_as_yap_expr_4x

    COMMAND map_assign_perf
    COMMAND arithmetic_perf
)
