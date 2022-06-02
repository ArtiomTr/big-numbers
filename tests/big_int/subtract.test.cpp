#include "BigInt.hpp"

#include "../utils.h"

// Testing sum of numbers which require only single cell.
bool testSingleNumber() {
    BigInt<uint8_t> one({2}, 0);
    BigInt<uint8_t> two({1}, 0);

    BigInt<uint8_t> result = one - two;

    return testBigInt(result, BigInt({1}, 0));
}

// Testing sum of numbers which require 2 cells.
bool testShortNumbers() {
    BigInt<uint8_t> one({0b00001001, 0b00000001}, 0); // 265
    BigInt<uint8_t> two({0b00010000, 0b10000000}, 0); // 32784

    BigInt<uint8_t> result = one - two;

    return testBigInt(result, BigInt({0b11111001, 0b10000000}, 1)); // -32519
}

// Testing sum of numbers which require 5 cells.
bool testLongerNumbers() {
    BigInt<uint8_t> one({0b11001101, 0b11101001, 0b00001001, 0b11001001, 0b00110001}, 0); // 213826267597
    BigInt<uint8_t> two({0b00110000, 0b00010000, 0b01010010, 0b00010000, 0b10001000}, 0); // 584389365808

    BigInt<uint8_t> result = one - two;

    // -370563098211
    return testBigInt(result, BigInt({0b10011101, 0b11011001, 0b10110111, 0b10111000, 0b10101001}, 1));
}

// Testing sum of big numbers of different size.
bool testDiffSizedNumbers() {
    BigInt<uint8_t> one({0b00110001}, 0); // 49
    BigInt<uint8_t> two({0b10001000, 0b10001000}, 0); // 34952

    BigInt<uint8_t> result = one - two;

    return testBigInt(result, BigInt({0b10101001, 0b01110111}, 1)); // -34903
}

bool testEmptySubtraction() {
    BigInt<uint8_t> one({}, 0);
    BigInt<uint8_t> two({}, 0);

    BigInt<uint8_t> result = one - two;

    return testBigInt(result, BigInt({0}, 1));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single number",             testSingleNumber},
            {"Short number",              testShortNumbers},
            {"Longer number",             testLongerNumbers},
            {"Numbers of different size", testDiffSizedNumbers},
            {"Empty numbers",             testEmptySubtraction},
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}