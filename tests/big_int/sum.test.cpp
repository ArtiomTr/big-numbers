#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

// Testing sum of numbers which require only single cell.
bool testSingleNumber() {
    BigIntBackend<uint8_t> one({1}, false);
    BigIntBackend<uint8_t> two({2}, false);

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({3}, false));
}

// Testing sum of numbers which require 2 cells.
bool testShortNumbers() {
    BigIntBackend<uint8_t> one({0b00001001, 0b00000001}, false); // 265
    BigIntBackend<uint8_t> two({0b00010000, 0b10000000}, false); // 32784

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00011001, 0b10000001}, false)); // 33049
}

// Testing sum of numbers which require 5 cells.
bool testLongerNumbers() {
    BigIntBackend<uint8_t> one(
            {0b11001101, 0b11101001, 0b00001001, 0b11001001, 0b00110001}, false); // 213826267597
    BigIntBackend<uint8_t> two(
            {0b00110000, 0b00010000, 0b01010010, 0b00010000, 0b10001000}, false); // 584389365808

    one.add(two);

    // 798215633405
    return testBigInt(one, BigIntBackend<uint8_t>({0b11111101, 0b11111001, 0b01011011, 0b11011001, 0b10111001}, false));
}

// Testing sum of big numbers of different size.
bool testDiffSizedNumbers() {
    BigIntBackend<uint8_t> one({0b00110001}, false); // 49
    BigIntBackend<uint8_t> two({0b10001000, 0b10001000}, false); // 34952

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b10111001, 0b10001000}, false)); // 35001
}

// Testing sum carry inside one cell.
bool testCarry() {
    BigIntBackend<uint8_t> one({0b00000001}, false); // 1
    BigIntBackend<uint8_t> two({0b00000001}, false); // 1

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000010}, false)); // 2
}

// Testing sum carry between two cells.
bool testLongCarry() {
    BigIntBackend<uint8_t> one({0b10000000, 0b00000000}, false); // 128
    BigIntBackend<uint8_t> two({0b10000000, 0b00000000}, false); // 128

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00000001}, false)); // 256
}

// Testing cell creation when carrying on last cell.
bool testCellCreation() {
    BigIntBackend<uint8_t> one({0b10000000}, false); // 128
    BigIntBackend<uint8_t> two({0b10000000}, false); // 128

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00000001}, false)); // 256
}

// Testing cell creation when carrying on last cell.
bool testFullCellCarry() {
    BigIntBackend<uint8_t> one({0b10000000, 0b11111111, 0b00000000}, false); // 65408
    BigIntBackend<uint8_t> two({0b10000000, 0b11111111, 0b00000000}, false); // 65408

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b11111111, 0b00000001}, false)); // 130816
}

bool testEmptySum() {
    BigIntBackend<uint8_t> one;
    BigIntBackend<uint8_t> two;

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>());
}

bool testSumWithZero() {
    BigIntBackend<uint8_t> one;
    BigIntBackend<uint8_t> two({0b10101010}, false);

    one.add(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b10101010}, false));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single number",                  testSingleNumber},
            {"Short number",                   testShortNumbers},
            {"Longer number",                  testLongerNumbers},
            {"Numbers of different size",      testDiffSizedNumbers},
            {"Sum carrying (inside cell)",     testCarry},
            {"Sum carrying (between cells)",   testLongCarry},
            {"Sum carrying (cell creation)",   testCellCreation},
            {"Sum carrying (full cell carry)", testFullCellCarry},
            {"Empty numbers",                  testEmptySum},
            {"Sum with zero",                  testSumWithZero}
    };

    for (const auto&[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}