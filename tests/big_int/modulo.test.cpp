#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigIntBackend<uint8_t> one(false, {0b00000110});
    BigIntBackend<uint8_t> two(false, {0b00000011});

    BigIntBackend<uint8_t> result = one.divide(two);

    return testBigInt(result, BigIntBackend<uint8_t>());
}

bool testMultiplePieces() {
    BigIntBackend<uint8_t> one(false, {0b11101000, 0b00000011});
    BigIntBackend<uint8_t> two(false, {0b11010011, 0b00000010});

    BigIntBackend<uint8_t> result = one.divide(two);

    return testBigInt(result, BigIntBackend<uint8_t>(false, {0b00010101, 0b00000001}));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",               testSimple},
            {"Test with multiple pieces", testMultiplePieces},
    };


    return runTests(tests);
}