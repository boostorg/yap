find_program(lldb lldb)
find_program(gdb gdb)

include_directories(${CMAKE_HOME_DIRECTORY})

macro(add_code_gen_executable name)
    add_executable(${name} ${name}.cpp)
    if (clang_on_linux)
        target_link_libraries(${name} c++)
    endif ()
endmacro()

add_code_gen_executable(code_gen_samples)
add_code_gen_executable(map_assign_code_gen)


if (lldb)
    add_custom_target(
        perf
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_cpp_expr" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_proto_expr" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_cpp_expr_4x" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_proto_expr_4x" -o quit

            COMMAND lldb -f $<TARGET_FILE:map_assign_code_gen> -o "disassemble --name make_map_with_boost_yap" -o quit
            COMMAND lldb -f $<TARGET_FILE:map_assign_code_gen> -o "disassemble --name make_map_with_boost_assign" -o quit
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
    )
endif()
