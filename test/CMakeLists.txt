include(CTest)

enable_testing()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -VV -C ${CMAKE_CFG_INTDIR})

include_directories(
    ${CMAKE_HOME_DIRECTORY}
    ${CMAKE_HOME_DIRECTORY}/googletest-release-1.8.0/googletest/include
)

macro(add_test_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} gtest gtest_main)
    add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name} --gtest_catch_exceptions=1)
endmacro()

add_test_executable(print)
add_test_executable(user_expression_transform)
add_test_executable(placeholder_eval)
add_test_executable(call_expr)
add_test_executable(reference_returns)
add_test_executable(depth_stress_test)
