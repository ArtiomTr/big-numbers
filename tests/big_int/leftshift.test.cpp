#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> one({0b00000001}, 0);

    one.shiftLeft(1);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000010}, 0));
}

bool testFullBlock() {
    BigIntBackend<uint8_t> one({0b11111111}, 0);

    one.shiftLeft(1);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11111110, 0b00000001}, 0));
}

bool testMultipleShift() {
    BigIntBackend<uint8_t> one({0b11111111}, 0);

    one.shiftLeft(4);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11110000, 0b00001111}, 0));
}

bool testInsertion() {
    BigIntBackend<uint8_t> one({0b11111111}, 0);

    one.shiftLeft(16);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00000000, 0b11111111}, 0));
}

bool testWithRemainder() {
    BigIntBackend<uint8_t> one({0b11111111}, 0);

    one.shiftLeft(20);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00000000, 0b11110000, 0b00001111}, 0));
}

bool testNegativeNumber() {
    BigIntBackend<uint8_t> one({0b11111110}, 1);

    one.shiftLeft(2);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11111000}, 1));
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