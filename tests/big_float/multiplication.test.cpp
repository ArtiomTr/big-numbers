#include "BigFloatBackend.h"

#include <iostream>

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>(false, {0b11101000}), -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>(false, {0b11101000}), -1); // 0.90625

    first.multiply(second, 2);

    BigIntBackend<uint8_t> mantissa(false, {0b01000000, 0b11010010}); // 0.8212890625
    int32_t exponent = -1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testZero() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>(false, {0b11101000}), -1);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>(), 0);

    first.multiply(second, 2);
    BigIntBackend<uint8_t> mantissa;
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool shouldNormalizeOutput() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>(false, {0b11101000}), 0);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>(false, {0b11101000}), 0);

    first.multiply(second, 2);

    BigIntBackend<uint8_t> mantissa(false, {0b01000000, 0b11010010});
    int32_t exponent = 1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Should multiply",         testSimple},
            {"Multiply by zero",        testZero},
            {"Should normalize output", shouldNormalizeOutput},
    };

    return runTests(tests);
}