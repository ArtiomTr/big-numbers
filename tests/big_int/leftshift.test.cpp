#include "BigInt.hpp"

#include "../utils.h"

bool testSingleCell() {
    BigInt<uint8_t> one({0b00000001}, 0);

    BigInt<uint8_t> result = one << 1;

    return testBigInt(result, BigInt({0b00000010}, 0));
}

bool testFullBlock() {
    BigInt<uint8_t> one({0b11111111}, 0);

    BigInt<uint8_t> result = one << 1;

    return testBigInt(result, BigInt({0b11111110, 0b00000001}, 0));
}

bool testMultipleShift() {
    BigInt<uint8_t> one({0b11111111}, 0);

    BigInt<uint8_t> result = one << 4;

    return testBigInt(result, BigInt({0b11110000, 0b00001111}, 0));
}

bool testInsertion() {
    BigInt<uint8_t> one({0b11111111}, 0);

    BigInt<uint8_t> result = one << 16;

    return testBigInt(result, BigInt({0b00000000, 0b00000000, 0b11111111}, 0));
}

bool testWithRemainder() {
    BigInt<uint8_t> one({0b11111111}, 0);

    BigInt<uint8_t> result = one << 20;

    return testBigInt(result, BigInt({0b00000000, 0b00000000, 0b11110000, 0b00001111}, 0));
}

bool testNegativeNumber() {
    BigInt<uint8_t> one({0b11111110}, 1);

    BigInt<uint8_t> result = one << 2;

    return testBigInt(result, BigInt({0b11111000}, 1));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell",              testSingleCell},
            {"Full block shift",         testFullBlock},
            {"Multiple shift",           testMultipleShift},
            {"Insertion",                testInsertion},
            {"Insertion with remainder", testWithRemainder},
            {"Negative",                 testNegativeNumber},
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}