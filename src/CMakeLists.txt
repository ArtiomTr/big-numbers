file(GLOB SRC_FILES *.cpp)

set(CMAKE_C_FLAGS_RELEASE "-O3")

add_library(big_numbers ${SRC_FILES})