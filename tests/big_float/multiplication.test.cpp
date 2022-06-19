#include "BigFloatBackend.h"

#include <iostream>

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), 2, -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, false), 2, -1); // 0.90625

    first.multiply(second);

    BigIntBackend<uint8_t> mantissa({0b01000000, 0b11010010}, false); // 0.8212890625
    int32_t exponent = -1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
}

bool testZero() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), 2, -1);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>(), 2, 0);

    first.multiply(second);
    BigIntBackend<uint8_t> mantissa;
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
}

bool shouldNormalizeOutput() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), 2, 0);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, false), 2, 0);

    first.multiply(second);

    BigIntBackend<uint8_t> mantissa({0b01000000, 0b11010010}, false);
    int32_t exponent = 1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
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