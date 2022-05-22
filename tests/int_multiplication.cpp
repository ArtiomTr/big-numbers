#include "big_int.hpp"

#include "utils.h"

#include <chrono>
#include <random>

bool testSingleCell() {
    big_int<uint8_t> one({0b00000010}, 0);
    big_int<uint8_t> two({0b00000010}, 0);

    big_int<uint8_t> result = one * two;

    return test_big_int(result, big_int({0b00000100}, 0));
}

bool testMultipleCells() {
    big_int<uint8_t> one({0b10000000}, 0);
    big_int<uint8_t> two({0b01000000}, 0);

    big_int<uint8_t> result = one * two;

    return test_big_int(result, big_int({0b00000000, 0b00100000}, 0));
}

bool testFilledCells() {
    big_int<uint8_t> one({0b11111111}, 0);
    big_int<uint8_t> two({0b11111111}, 0);

    big_int<uint8_t> result = one * two;

    return test_big_int(result, big_int({0b00000001, 0b11111110}, 0));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell",    testSingleCell},
            {"Multiple cells", testMultipleCells},
            {"Filled cells",   testFilledCells}
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}