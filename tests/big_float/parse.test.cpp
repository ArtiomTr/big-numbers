#include "big_float.hpp"

#include <iostream>

#include "../utils.h"

bool testNoFractional() {
    big_float<uint8_t> num = parse_big_float<uint8_t>("1000.0", 10);

    return test_big_float(num, big_float<uint8_t>(big_int<uint8_t>({0b11110100}, 0), 8));
}

bool testZero() {
    big_float<uint8_t> num = parse_big_float<uint8_t>("0.0", 10);

    return test_big_float(num, big_float<uint8_t>(big_int<uint8_t>({}, 0), 0));
}

bool testOne() {
    big_float<uint8_t> num = parse_big_float<uint8_t>("1.0", 10);

    return test_big_float(num, big_float<uint8_t>(big_int<uint8_t>({}, 0), 1));
}

bool testFraction() {
    big_float<uint8_t> num = parse_big_float<uint8_t>("0.000976562", 10);

    return test_big_float(num, big_float<uint8_t>(big_int<uint8_t>({}, 0), -10));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"No fractional part",  testNoFractional},
            {"Test zero",           testZero},
            {"Test one",            testOne},
            {"Test small fraction", testFraction},
    };

    try {
        for (auto[testName, runTest]: tests) {
            std::cout << "Test case: \"" << testName << '"' << std::endl;
            if (!runTest()) {
                return 1;
            }
        }

    } catch (...) {
        return 1;
    }

    return 0;
}