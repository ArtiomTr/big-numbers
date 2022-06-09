#include "BigInt.hpp"

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b00000110}, 0);
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000011}, 0);

    BigInt<uint8_t> result = one % two;

    return testBigInt(result, BigInt());
}

bool testMultiplePieces() {
    BigInt<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b11101000, 0b00000011}, 0);
    BigInt<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b11010011, 0b00000010}, 0);

    BigInt<uint8_t> result = one % two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00010101, 0b00000001}, 0));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",               testSimple},
            {"Test with multiple pieces", testMultiplePieces},
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}