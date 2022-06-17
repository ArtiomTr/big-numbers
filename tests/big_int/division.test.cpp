#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigIntBackend<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b00000110}, 0);
    BigIntBackend<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000011}, 0);

    BigIntBackend<uint8_t> result = one / two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00000010}, 0));
}

bool testMultiplePieces() {
    BigIntBackend<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b10010001, 0b00000110}, 0);
    BigIntBackend<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000011}, 0);

    BigIntBackend<uint8_t> result = one / two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b00110000, 0b00000010}, 0));
}

bool testFlooring() {
    BigIntBackend<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b10010001, 0b00000110}, 0);
    BigIntBackend<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000011}, 0);

    BigIntBackend<uint8_t> result = two / one;

    return testBigInt(result, BigIntBackend());
}

bool testNegative() {
    BigIntBackend<uint8_t> one = BigIntDebugger<uint8_t>::createFromSource({0b11111010}, 1);
    BigIntBackend<uint8_t> two = BigIntDebugger<uint8_t>::createFromSource({0b00000011}, 0);

    BigIntBackend<uint8_t> result = one / two;

    return testBigInt(result, BigIntDebugger<uint8_t>::createFromSource({0b11111110}, 1));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",               testSimple},
            {"Test with multiple pieces", testMultiplePieces},
            {"Test flooring",             testFlooring},
            {"Test negative",             testNegative},
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}