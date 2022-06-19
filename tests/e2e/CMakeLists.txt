set(TEST_PREFIX "e2e")

file(GLOB TEST_FILES *.test.cpp)

include_directories(../../src)

foreach (TEST_PATH IN LISTS TEST_FILES)
    string(REGEX MATCH "([^\\/]+)\\.test\\.cpp$" _ "${TEST_PATH}")

    set(TEST_NAME "${TEST_PREFIX}_${CMAKE_MATCH_1}")

    add_executable("${TEST_NAME}" "${TEST_PATH}")
    target_link_libraries(${TEST_NAME} big_numbers)
    add_test(NAME "${TEST_NAME}" COMMAND ${TEST_NAME})
endforeach ()