#include "BigFloat.hpp"

#include <iostream>

#include "../utils.h"

bool testSimple() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625
    BigFloat<uint8_t> second(BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625

    BigFloat<uint8_t> out = first * second;

    BigInt<uint8_t> mantissa({0b01000000, 0b11010010}, 0); // 0.8212890625
    int32_t exponent = -1;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));
}

bool testZero() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b11101000}, 0), -1);
    BigFloat<uint8_t> second(BigInt<uint8_t>({}, 0), 0);

    BigFloat<uint8_t> out = first * second;

    BigInt<uint8_t> mantissa({}, 0);
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));
}

bool shouldNormalizeOutput() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b11101000}, 0), 0);
    BigFloat<uint8_t> second(BigInt<uint8_t>({0b11101000}, 0), 0);

    BigFloat<uint8_t> out = first * second;

    BigInt<uint8_t> mantissa({0b01000000, 0b11010010}, 0);
    int32_t exponent = 1;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Should multiply",         testSimple},
            {"Multiply by zero",        testZero},
            {"Should normalize output", shouldNormalizeOutput},
    };

    try {
        for (auto[testName, runTest]: tests) {
            std::cout << "Test case: \"" << testName << '"' << std::endl;
            if (!runTest()) {
                return 1;
            }
        }

    } catch (...) {
        std::cout << "Unexpected exception" << std::endl;
        return 1;
    }

    return 0;
}