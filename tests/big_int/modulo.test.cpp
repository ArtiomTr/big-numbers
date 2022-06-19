#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigIntBackend<uint8_t> one({0b00000110}, false);
    BigIntBackend<uint8_t> two({0b00000011}, false);

    BigIntBackend<uint8_t> result = one.divide(two);

    return testBigInt(result, BigIntBackend<uint8_t>());
}

bool testMultiplePieces() {
    BigIntBackend<uint8_t> one({0b11101000, 0b00000011}, false);
    BigIntBackend<uint8_t> two({0b11010011, 0b00000010}, false);

    BigIntBackend<uint8_t> result = one.divide(two);

    return testBigInt(result, BigIntBackend<uint8_t>({0b00010101, 0b00000001}, false));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",               testSimple},
            {"Test with multiple pieces", testMultiplePieces},
    };


    return runTests(tests);
}