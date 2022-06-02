#include "BigFloat.hpp"

#include <iostream>

#include "../utils.h"

bool testNoFractional() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("1000.0333", 4);

    BigInt<uint8_t> mantissa({0b10000110, 0b00001000, 0b11101000, 0b00000011}, 0);
    int32_t exponent = 1;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testZero() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("0.0000000000000000000000", 10);

    BigInt<uint8_t> mantissa({}, 0);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testOne() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("1.0", 10);

    BigInt<uint8_t> mantissa({0b00000001}, 0);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testFraction() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("0.000976562", 10);

    BigInt<uint8_t> mantissa({0b01101010, 0b00101101, 0b00100101, 0b11111011, 0b10000010, 0b00111110, 0b11011010,
                              0b11111101, 0b11111111, 0b00111111}, 0);
    int32_t exponent = -1;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testRounding() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("0.999999940395355224609375", 2);

    BigInt<uint8_t> mantissa({0b00000001}, 0);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testRounding2() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("0.00390624976716935634613037109375", 2);

    BigInt<uint8_t> mantissa({0b00000001}, 0);
    int32_t exponent = -1;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testRounding3() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("1.0", 2);

    BigInt<uint8_t> mantissa({0b00000001}, 0);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testStripZeros() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("0.00001417635940015316009521484375", 2);

    BigInt<uint8_t> mantissa({0b11010111, 0b11101101}, 0);
    int32_t exponent = -2;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
}

bool testNegative() {
    BigFloat<uint8_t> num = parseBigFloat<uint8_t>("-1.0", 2);

    BigInt<uint8_t> mantissa({0b11111111}, 1);
    int32_t exponent = 0;

    return testBigFloat(num, BigFloat<uint8_t>(mantissa, exponent));
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