#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigIntBackend<uint8_t> one({0b00000110}, false);
    BigIntBackend<uint8_t> two({0b00000011}, false);

    one.divide(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00000010}, false));
}

bool testMultiplePieces() {
    BigIntBackend<uint8_t> one({0b10010001, 0b00000110}, false);
    BigIntBackend<uint8_t> two({0b00000011}, false);

    one.divide(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b00110000, 0b00000010}, false));
}

bool testFlooring() {
    BigIntBackend<uint8_t> one({0b10010001, 0b00000110}, false);
    BigIntBackend<uint8_t> two({0b00000011}, false);

    two.divide(one);

    return testBigInt(two, BigIntBackend<uint8_t>());
}

bool testNegative() {
    BigIntBackend<uint8_t> one({0b11111010}, true);
    BigIntBackend<uint8_t> two({0b00000011}, false);

    one.divide(two);

    return testBigInt(one, BigIntBackend<uint8_t>({0b11111110}, true));
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