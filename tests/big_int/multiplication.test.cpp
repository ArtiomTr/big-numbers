#include "BigIntBackend.h"

#include "../utils.h"

#include <chrono>
#include <random>

using namespace BigNumbers;

// TODO: add negative value tests
bool testSingleCell() {
    BigIntBackend<uint8_t> one(false, {0b00000010});
    BigIntBackend<uint8_t> two(false, {0b00000010});

    one.multiply(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000100}));
}

bool testMultipleCells() {
    BigIntBackend<uint8_t> one(false, {0b10000000});
    BigIntBackend<uint8_t> two(false, {0b01000000});

    one.multiply(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000000, 0b00100000}));
}

bool testFilledCells() {
    BigIntBackend<uint8_t> one(false, {0b11111111});
    BigIntBackend<uint8_t> two(false, {0b11111111});

    one.multiply(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000001, 0b11111110}));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell",    testSingleCell},
            {"Multiple cells", testMultipleCells},
            {"Filled cells",   testFilledCells}
    };


    return runTests(tests);
}