cmake_minimum_required(VERSION 3.20)
project(big_numbers)

set(CMAKE_CXX_STANDARD 20)

add_executable(big_numbers main.cpp ./src/parsing_utils.cpp ./src/big_int.cpp ./src/big_float.cpp ./src/padding.cpp)
