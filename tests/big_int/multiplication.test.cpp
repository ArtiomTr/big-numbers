#include "BigIntBackend.h"

#include "../utils.h"

#include <chrono>
#include <random>

using namespace BigNumbers;

// TODO: add negative value tests
bool testSingleCell() {
    BigIntBackend<uint8_t> one({0b00000010}, false);
    BigIntBackend<uint8_t> two({0b00000010}, false);

    one.multiply(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000100}, false));
}

bool testMultipleCells() {
    BigIntBackend<uint8_t> one({0b10000000}, false);
    BigIntBackend<uint8_t> two({0b01000000}, false);

    one.multiply(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000000, 0b00100000}, false));
}

bool testFilledCells() {
    BigIntBackend<uint8_t> one({0b11111111}, false);
    BigIntBackend<uint8_t> two({0b11111111}, false);

    one.multiply(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000001, 0b11111110}, false));
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