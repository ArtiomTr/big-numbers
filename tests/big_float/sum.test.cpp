#include "BigFloatBackend.h"
#include "../utils.h"

#include <iostream>

using namespace BigNumbers;

bool testFractionAlignment() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, 0), 3, -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, 0), 3, 1); // 59392

    first.add(second);

    BigIntBackend<uint8_t> mantissa({0b11101000, 0b00000000, 0b11101000},
                                    0); // 59392.90625
    int32_t exponent = 1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 3, exponent));
}

bool testExponent() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, 0), 2, -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, 0), 2, -1); // 0.90625

    first.add(second);

    BigIntBackend<uint8_t> mantissa = BigIntBackend<uint8_t>({0b11010000, 0b00000001}, 0); // 1.8125
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
}

bool testNegative() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, 0), 2, -1); // 0.90625
    BigIntBackend<uint8_t> secondMantissa({0b11101000}, 0);
    secondMantissa.negate();
    BigFloatBackend<uint8_t> second(secondMantissa, 2, -1); // 0.90625

    first.add(second);

    BigIntBackend<uint8_t> mantissa; // 0
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
}

bool testMemoryOptimization() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b0100000, 0b11001100}, 0), 2, 0); // 0.90625
    BigIntBackend<uint8_t> secondMantissa({0b0100000}, 0);
    secondMantissa.negate();
    BigFloatBackend<uint8_t> second(secondMantissa, 2, -1); // 0.90625

    first.add(second);

    BigIntBackend<uint8_t> mantissa({0b11001100}, 0); // 0
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
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