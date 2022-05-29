cmake_minimum_required(VERSION 3.20)
project(big_numbers)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
add_subdirectory(debug)
