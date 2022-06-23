#include "BigIntBackend.h"

#include <sstream>

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> value(false, {0b00000110});

    return value.toString() == "6";
}

bool testMultipleCells() {
    BigIntBackend<uint8_t> value(false, {0b00000000, 0b00001100});

    return value.toString() == "3072";
}

bool testLargeValue() {
    BigIntBackend<uint8_t> value(false,
                                 {0b00100100, 0b00001100, 0b11011000, 0b00110011,
                                  0b11001100, 0b00010001, 0b11100011, 0b00100100,
                                  0b10101010, 0b00000001});

    return value.toString() == "7860970963174088510500";
}

bool testZero() {
    BigIntBackend<uint8_t> value;

    return value.toString() == "0";
}

bool testNegative() {
    BigIntBackend<uint8_t> value(true, {0b11111010});

    return value.toString() == "-6";
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell",         testSingleCell},
            {"Test multiple cells", testMultipleCells},
            {"Test large value",    testLargeValue},
            {"Test zero",           testZero},
            {"Test negative",       testNegative}
    };


    return runTests(tests);
}