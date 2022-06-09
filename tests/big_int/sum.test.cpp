#include "BigInt.hpp"

#include "../utils.h"

using namespace BigNumbers;

// Testing sum of numbers which require only single cell.
bool testSingleNumber() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({1}, 0);
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({2}, 0);

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({3}, 0));
}

// Testing sum of numbers which require 2 cells.
bool testShortNumbers() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b00001001, 0b00000001}, 0); // 265
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00010000, 0b10000000}, 0); // 32784

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00011001, 0b10000001}, 0)); // 33049
}

// Testing sum of numbers which require 5 cells.
bool testLongerNumbers() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource(
            {0b11001101, 0b11101001, 0b00001001, 0b11001001, 0b00110001}, 0); // 213826267597
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource(
            {0b00110000, 0b00010000, 0b01010010, 0b00010000, 0b10001000}, 0); // 584389365808

    BigInt<uint8_t> result = one + two;

    // 798215633405
    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource(
            {0b11111101, 0b11111001, 0b01011011, 0b11011001, 0b10111001}, 0));
}

// Testing sum of big numbers of different size.
bool testDiffSizedNumbers() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b00110001}, 0); // 49
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b10001000, 0b10001000}, 0); // 34952

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b10111001, 0b10001000}, 0)); // 35001
}

// Testing sum carry inside one cell.
bool testCarry() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b00000001}, 0); // 1
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000001}, 0); // 1

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000010}, 0)); // 2
}

// Testing sum carry between two cells.
bool testLongCarry() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b10000000, 0b00000000}, 0); // 128
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b10000000, 0b00000000}, 0); // 128

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000000, 0b00000001}, 0)); // 256
}

// Testing cell creation when carrying on last cell.
bool testCellCreation() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b10000000}, 0); // 128
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b10000000}, 0); // 128

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000000, 0b00000001}, 0)); // 256
}

// Testing cell creation when carrying on last cell.
bool testFullCellCarry() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b10000000, 0b11111111, 0b00000000}, 0); // 65408
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b10000000, 0b11111111, 0b00000000}, 0); // 65408

    BigInt<uint8_t> result = one + two;

    return testBigInt(result,
                      BigIntDebugger<uint8_t>::createFromSource({0b00000000, 0b11111111, 0b00000001}, 0)); // 130816
}

bool testEmptySum() {
    BigInt<uint8_t> one;
    BigInt<uint8_t> two;

    BigInt<uint8_t> result = one + two;

    return testBigInt(result, BigInt());
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
            {"Empty numbers",                  testEmptySum}
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}