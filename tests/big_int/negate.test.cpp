#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> one(false, {0b00000010});

    one.negate();

    return testBigInt(one, BigIntBackend<uint8_t>(true, {0b11111110}));
}

bool testZero() {
    BigIntBackend<uint8_t> zero(false, {0b00000000});

    zero.negate();

    return testBigInt(zero, BigIntBackend<uint8_t>(false, {0b00000000}));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell", testSingleCell},
            {"Test zero",   testZero}
    };


    return runTests(tests);
}