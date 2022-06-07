#include "BigFloat.hpp"

#include <iostream>

#include "../utils.h"

bool testFractionAlignment() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625
    BigFloat<uint8_t> second(BigInt<uint8_t>({0b11101000}, 0), 1); // 59392

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa({0b11101000, 0b00000000, 0b11101000}, 0); // 59392.90625
    int32_t exponent = 1;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));
}

bool testExponent() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625
    BigFloat<uint8_t> second(BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa({0b11010000, 0b00000001}, 0); // 1.8125
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));
}

bool testNegative() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625
    BigFloat<uint8_t> second(-BigInt<uint8_t>({0b11101000}, 0), -1); // 0.90625

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa({}, 0); // 0
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));
}

bool testMemoryOptimization() {
    BigFloat<uint8_t> first(BigInt<uint8_t>({0b0100000, 0b11001100}, 0), 0); // 0.90625
    BigFloat<uint8_t> second(-BigInt<uint8_t>({0b0100000}, 0), -1); // 0.90625

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa({0b11001100}, 0); // 0
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, exponent));

}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Fraction alignment",        testFractionAlignment},
            {"Test exponent computation", testExponent},
            {"Test negative",             testNegative},
            {"Test memory optimization",  testMemoryOptimization},
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