find_program(lldb lldb)
find_program(gdb gdb)

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


if (lldb)
    add_custom_target(
        perf
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_cpp_expr" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_proto_expr" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_cpp_expr_4x" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_proto_expr_4x" -o quit

            COMMAND lldb -f $<TARGET_FILE:map_assign_code_gen> -o "disassemble --name make_map_with_boost_yap" -o quit
            COMMAND lldb -f $<TARGET_FILE:map_assign_code_gen> -o "disassemble --name make_map_with_boost_assign" -o quit
            COMMAND lldb -f $<TARGET_FILE:map_assign_code_gen> -o "disassemble --name make_map_manually" -o quit
            COMMAND lldb -f $<TARGET_FILE:map_assign_code_gen> -o "disassemble --name make_map_inializer_list" -o quit

            COMMAND map_assign_perf
            COMMAND arithmetic_perf
    )
elseif (gdb)
    add_custom_target(
        perf
            COMMAND gdb -se $<TARGET_FILE:code_gen_samples> -batch -ex "disassemble eval_as_cpp_expr"
            COMMAND gdb -se $<TARGET_FILE:code_gen_samples> -batch -ex "disassemble eval_as_proto_expr"
            COMMAND gdb -se $<TARGET_FILE:code_gen_samples> -batch -ex "disassemble eval_as_cpp_expr_4x"
            COMMAND gdb -se $<TARGET_FILE:code_gen_samples> -batch -ex "disassemble eval_as_proto_expr_4x"

            COMMAND gdb -se $<TARGET_FILE:map_assign_code_gen> -batch -ex "disassemble make_map_with_boost_yap"
            COMMAND gdb -se $<TARGET_FILE:map_assign_code_gen> -batch -ex "disassemble make_map_with_boost_assign"
            COMMAND gdb -se $<TARGET_FILE:map_assign_code_gen> -batch -ex "disassemble make_map_manually"
            COMMAND gdb -se $<TARGET_FILE:map_assign_code_gen> -batch -ex "disassemble make_map_inializer_list"

            COMMAND map_assign_perf
            COMMAND arithmetic_perf
    )
endif()
