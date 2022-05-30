add_executable(int_sum int_sum.cpp)
target_link_libraries(int_sum big_numbers big_numbers_debugger)
add_test(NAME "big_int sum tests" COMMAND int_sum)

add_executable(int_subtract int_subtract.cpp)
target_link_libraries(int_subtract big_numbers big_numbers_debugger)
add_test(NAME "big_int subtract tests" COMMAND int_subtract)

add_executable(int_leftshift int_leftshift.cpp)
target_link_libraries(int_leftshift big_numbers big_numbers_debugger)
add_test(NAME "big_int left bitshift tests" COMMAND int_leftshift)

add_executable(int_multiplication int_multiplication.cpp)
target_link_libraries(int_multiplication big_numbers big_numbers_debugger)
add_test(NAME "big_int multiplication tests" COMMAND int_multiplication)

add_executable(int_division int_division.cpp)
target_link_libraries(int_division big_numbers big_numbers_debugger)
add_test(NAME "big_int division tests" COMMAND int_division)

add_executable(float_parse float_parse.cpp)
target_link_libraries(float_parse big_numbers big_numbers_debugger)
add_test(NAME "big_float parse tests" COMMAND float_parse)

include_directories(../src)
include_directories(../debug)
