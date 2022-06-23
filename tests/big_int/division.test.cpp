#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigIntBackend<uint8_t> one(false, {0b00000110});
    BigIntBackend<uint8_t> two(false, {0b00000011});

    one.divide(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00000010}));
}

bool testMultiplePieces() {
    BigIntBackend<uint8_t> one(false, {0b10010001, 0b00000110});
    BigIntBackend<uint8_t> two(false, {0b00000011});

    one.divide(two);

    return testBigInt(one, BigIntBackend<uint8_t>(false, {0b00110000, 0b00000010}));
}

bool testFlooring() {
    BigIntBackend<uint8_t> one(false, {0b10010001, 0b00000110});
    BigIntBackend<uint8_t> two(false, {0b00000011});

    two.divide(one);

    return testBigInt(two, BigIntBackend<uint8_t>());
}

bool testNegative() {
    BigIntBackend<uint8_t> one(true, {0b11111010});
    BigIntBackend<uint8_t> two(false, {0b00000011});

    one.divide(two);

    return testBigInt(one, BigIntBackend<uint8_t>(true, {0b11111110}));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",               testSimple},
            {"Test with multiple pieces", testMultiplePieces},
            {"Test flooring",             testFlooring},
            {"Test negative",             testNegative},
    };


    return runTests(tests);
}