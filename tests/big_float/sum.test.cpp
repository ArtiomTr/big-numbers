#include "BigFloat.hpp"
#include "../utils.h"

#include <iostream>

using namespace BigNumbers;

bool testFractionAlignment() {
    BigFloat<uint8_t> first(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 3, -1); // 0.90625
    BigFloat<uint8_t> second(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 3, 1); // 59392

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa = BigIntDebugger<uint8_t>::createFromSource({0b11101000, 0b00000000, 0b11101000},
                                                                         0); // 59392.90625
    int32_t exponent = 1;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, 3, exponent));
}

bool testExponent() {
    BigFloat<uint8_t> first(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 2, -1); // 0.90625
    BigFloat<uint8_t> second(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 2, -1); // 0.90625

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa = BigIntDebugger<uint8_t>::createFromSource({0b11010000, 0b00000001}, 0); // 1.8125
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, 2, exponent));
}

bool testNegative() {
    BigFloat<uint8_t> first(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 2, -1); // 0.90625
    BigFloat<uint8_t> second(-BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 2, -1); // 0.90625

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa; // 0
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, 2, exponent));
}

bool testMemoryOptimization() {
    BigFloat<uint8_t> first(BigIntDebugger<uint8_t>::createFromSource({0b0100000, 0b11001100}, 0), 2, 0); // 0.90625
    BigFloat<uint8_t> second(-BigIntDebugger<uint8_t>::createFromSource({0b0100000}, 0), 2, -1); // 0.90625

    BigFloat<uint8_t> out = first + second;

    BigInt<uint8_t> mantissa = BigIntDebugger<uint8_t>::createFromSource({0b11001100}, 0); // 0
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, 2, exponent));

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