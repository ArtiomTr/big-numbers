#include "BigInt.hpp"

#include "../utils.h"

#include <chrono>
#include <random>

using namespace BigNumbers;

// TODO: add negative value tests
bool testSingleCell() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b00000010}, 0);
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000010}, 0);

    BigInt<uint8_t> result = one * two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000100}, 0));
}

bool testMultipleCells() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b10000000}, 0);
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b01000000}, 0);

    BigInt<uint8_t> result = one * two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000000, 0b00100000}, 0));
}

bool testFilledCells() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b11111111}, 0);
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b11111111}, 0);

    BigInt<uint8_t> result = one * two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000001, 0b11111110}, 0));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell",    testSingleCell},
            {"Multiple cells", testMultipleCells},
            {"Filled cells",   testFilledCells}
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}