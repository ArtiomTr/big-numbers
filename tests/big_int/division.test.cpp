#include "big_int.hpp"

#include "../utils.h"

bool testSimple() {
    big_int<uint8_t> one({0b00000110}, 0);
    big_int<uint8_t> two({0b00000011}, 0);

    big_int<uint8_t> result = one / two;

    return test_big_int(result, big_int({0b00000010}, 0));
}

bool testMultiplePieces() {
    big_int<uint8_t> one({0b10010001, 0b00000110}, 0);
    big_int<uint8_t> two({0b00000011}, 0);

    big_int<uint8_t> result = one / two;

    return test_big_int(result, big_int({0b00110000, 0b00000010}, 0));
}

bool testFlooring() {
    big_int<uint8_t> one({0b10010001, 0b00000110}, 0);
    big_int<uint8_t> two({0b00000011}, 0);

    big_int<uint8_t> result = two / one;

    return test_big_int(result, big_int({}, 0));
}

bool testNegative() {
    big_int<uint8_t> one({0b11111010}, 1);
    big_int<uint8_t> two({0b00000011}, 0);

    std::cout << (one < 0 ? "true" : "false") << std::endl;

    big_int<uint8_t> result = one / two;

    return test_big_int(result, big_int({0b11111110}, 1));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",               testSimple},
            {"Test with multiple pieces", testMultiplePieces},
            {"Test flooring",             testFlooring},
            {"Test negative",             testNegative},
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}