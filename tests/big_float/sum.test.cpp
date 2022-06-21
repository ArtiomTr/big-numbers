#include "BigFloatBackend.h"
#include "../utils.h"

#include <iostream>

using namespace BigNumbers;

bool testFractionAlignment() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, false), 1); // 59392

    first.add(second);

    BigIntBackend<uint8_t> mantissa({0b11101000, 0b00000000, 0b11101000}, false); // 59392.90625
    int32_t exponent = 1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testExponent() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, false), -1); // 0.90625

    first.add(second);

    BigIntBackend<uint8_t> mantissa = BigIntBackend<uint8_t>({0b11010000, 0b00000001}, false); // 1.8125
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testNegative() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), -1); // 0.90625
    BigIntBackend<uint8_t> secondMantissa({0b11101000}, false);
    secondMantissa.negate();
    BigFloatBackend<uint8_t> second(secondMantissa, -1); // 0.90625

    first.add(second);

    BigIntBackend<uint8_t> mantissa; // 0
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testMemoryOptimization() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b0100000, 0b11001100}, false), 0); // 0.90625
    BigIntBackend<uint8_t> secondMantissa({0b0100000}, false);
    secondMantissa.negate();
    BigFloatBackend<uint8_t> second(secondMantissa, -1); // 0.90625

    first.add(second);

    BigIntBackend<uint8_t> mantissa({0b11001100}, false); // 0
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testNegative2() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b01010101, 0b10010010}, false), 0);
    BigIntBackend<uint8_t> secondMantissa({0b10010010}, false);
    secondMantissa.negate();
    BigFloatBackend<uint8_t> second(secondMantissa, 0);

    first.add(second);

    BigIntBackend<uint8_t> mantissa({0b01010101}, false);
    int32_t exponent = -1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Fraction alignment",        testFractionAlignment},
            {"Test exponent computation", testExponent},
            {"Test negative",             testNegative},
            {"Test negative 2",           testNegative2},
            {"Test memory optimization", testMemoryOptimization}
    };

    return runTests(tests);
}