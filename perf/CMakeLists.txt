add_custom_target(
    perf
        COMMAND 
)

include_directories(
    ${CMAKE_HOME_DIRECTORY}
)

add_executable(code_gen_samples code_gen_samples.cpp)
