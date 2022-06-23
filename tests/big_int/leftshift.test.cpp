#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> one(false, {0b00000001});

    one.shiftLeft(1);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000010}));
}

bool testFullBlock() {
    BigIntBackend<uint8_t> one(false, {0b11111111});

    one.shiftLeft(1);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b11111110, 0b00000001}));
}

bool testMultipleShift() {
    BigIntBackend<uint8_t> one(false, {0b11111111});

    one.shiftLeft(4);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b11110000, 0b00001111}));
}

bool testInsertion() {
    BigIntBackend<uint8_t> one(false, {0b11111111});

    one.shiftLeft(16);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000000, 0b00000000, 0b11111111}));
}

bool testWithRemainder() {
    BigIntBackend<uint8_t> one(false, {0b11111111});

    one.shiftLeft(20);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000000, 0b00000000, 0b11110000, 0b00001111}));
}

bool testNegativeNumber() {
    BigIntBackend<uint8_t> one(true, {0b11111110});

    one.shiftLeft(2);

    return testBigInt(one, BigIntBackend<uint8_t>(true, {0b11111000}));
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