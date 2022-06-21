#include <iostream>

#include "BigFloatBackend.h"
#include "../utils.h"
#include "ParsingUtils.h"

using namespace BigNumbers;

bool testNoFractional() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("1000.0333", 2);

    BigIntBackend<uint8_t> mantissa({0b10000110, 0b00001000, 0b11101000, 0b00000011}, false);
    int32_t exponent = 1;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testZero() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("0.0000000000000000000000", 10);

    BigIntBackend<uint8_t> mantissa;
    int32_t exponent = 0;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testOne() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("1.0", 10);

    BigIntBackend<uint8_t> mantissa({0b00000001}, false);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testFraction() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("0.000976562", 10);

    BigIntBackend<uint8_t> mantissa(
            {0b01101010, 0b00101101, 0b00100101, 0b11111011, 0b10000010, 0b00111110, 0b11011010,
             0b11111101, 0b11111111, 0b00111111}, false);
    int32_t exponent = -1;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testRounding() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("0.999999940395355224609375", 2);

    BigIntBackend<uint8_t> mantissa({0b00000001}, false);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testRounding2() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("0.00390624976716935634613037109375", 2);

    BigIntBackend<uint8_t> mantissa({0b00000001}, false);
    int32_t exponent = -1;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testRounding3() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("1.0", 2);

    BigIntBackend<uint8_t> mantissa({0b00000001}, false);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testStripZeros() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("0.00001417635940015316009521484375", 2);

    BigIntBackend<uint8_t> mantissa({0b11010111, 0b11101101}, false);
    int32_t exponent = -2;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

bool testNegative() {
    BigFloatBackend<uint8_t> num = parseBigFloat<uint8_t>("-1.0", 2);

    BigIntBackend<uint8_t> mantissa({0b11111111}, true);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloatBackend<uint8_t>(mantissa, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"No fractional part",    testNoFractional},
            {"Test zero",             testZero},
            {"Test one",              testOne},
            {"Test small fraction",   testFraction},
            {"Test rounding",         testRounding},
            {"Test rounding 2",       testRounding2},
            {"Test rounding 3",       testRounding3},
            {"Test strip zeros",      testStripZeros},
            {"Test negative numbers", testNegative}
    };

    return runTests(tests);
}