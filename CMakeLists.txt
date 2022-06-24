cmake_minimum_required(VERSION 3.20)
project(big_numbers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
