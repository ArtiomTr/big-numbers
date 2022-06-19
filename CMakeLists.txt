cmake_minimum_required(VERSION 3.20)
project(big_numbers)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

include_directories(src)
add_executable(btest demo.cpp)
target_link_libraries(btest big_numbers)

enable_testing()
add_subdirectory(tests)
