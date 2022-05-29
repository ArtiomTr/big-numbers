file(GLOB SRC_FILES *.cpp)

set(CMAKE_C_FLAGS_RELEASE "-O3")

include_directories(../src)
add_library(big_numbers_debugger ${SRC_FILES})
target_link_libraries(big_numbers_debugger big_numbers)
