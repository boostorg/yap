find_program(lldb lldb)
find_program(gdb gdb)

include_directories(${CMAKE_HOME_DIRECTORY})

add_executable(code_gen_samples code_gen_samples.cpp)


if (lldb)
    add_custom_target(
        perf
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_cpp_expr" -o quit
            COMMAND lldb -f $<TARGET_FILE:code_gen_samples> -o "disassemble --name eval_as_proto_expr" -o quit
    )
elseif (gdb)
    add_custom_target(
        perf
            COMMAND gdb -e $<TARGET_FILE:code_gen_samples> -batch -x "disassemble eval_as_cpp_expr"
            COMMAND gdb -e $<TARGET_FILE:code_gen_samples> -batch -x "disassemble eval_as_proto_expr"
    )
endif()
