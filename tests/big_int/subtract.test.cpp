#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

// Testing sum of numbers which require only single cell.
bool testSingleNumber() {
    BigIntBackend<uint8_t> one(false, {2});
    BigIntBackend<uint8_t> two(false, {1});

    one.subtract(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {1}));
}

// Testing sum of numbers which require 2 cells.
bool testShortNumbers() {
    BigIntBackend<uint8_t> one(false, {0b00001001, 0b00000001}); // 265
    BigIntBackend<uint8_t> two(false, {0b00010000, 0b10000000}); // 32784

    one.subtract(two);

    return testBigInt(one, BigIntBackend<uint8_t>(true, {0b11111001, 0b10000000})); // -32519
}

// Testing sum of numbers which require 5 cells.
bool testLongerNumbers() {
    BigIntBackend<uint8_t> one(false,
                               {0b11001101, 0b11101001, 0b00001001, 0b11001001, 0b00110001}); // 213826267597
    BigIntBackend<uint8_t> two(false,
                               {0b00110000, 0b00010000, 0b01010010, 0b00010000, 0b10001000}); // 584389365808

    one.subtract(two);

    // -370563098211
    return testBigInt(one, BigIntBackend<uint8_t>(true,
                                                  {0b10011101, 0b11011001, 0b10110111, 0b10111000, 0b10101001}));
}

// Testing sum of big numbers of different size.
bool testDiffSizedNumbers() {
    BigIntBackend<uint8_t> one(false, {0b00110001}); // 49
    BigIntBackend<uint8_t> two(false, {0b10001000, 0b10001000}); // 34952

    one.subtract(two);

    return testBigInt(one, BigIntBackend<uint8_t>(true, {0b10101001, 0b01110111})); // -34903
}

bool testEmptySubtraction() {
    BigIntBackend<uint8_t> one;
    BigIntBackend<uint8_t> two;

    one.subtract(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0}));
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


    return runTests(tests);
}