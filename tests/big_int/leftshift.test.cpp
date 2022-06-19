#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> one({0b00000001}, false);

    one.shiftLeft(1);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000010}, false));
}

bool testFullBlock() {
    BigIntBackend<uint8_t> one({0b11111111}, false);

    one.shiftLeft(1);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11111110, 0b00000001}, false));
}

bool testMultipleShift() {
    BigIntBackend<uint8_t> one({0b11111111}, false);

    one.shiftLeft(4);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11110000, 0b00001111}, false));
}

bool testInsertion() {
    BigIntBackend<uint8_t> one({0b11111111}, false);

    one.shiftLeft(16);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00000000, 0b11111111}, false));
}

bool testWithRemainder() {
    BigIntBackend<uint8_t> one({0b11111111}, false);

    one.shiftLeft(20);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00000000, 0b11110000, 0b00001111}, false));
}

bool testNegativeNumber() {
    BigIntBackend<uint8_t> one({0b11111110}, true);

    one.shiftLeft(2);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11111000}, true));
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


    return runTests(tests);
}