#include "BigFloatBackend.h"

#include <iostream>

#include "../utils.h"

#include <limits>

using namespace BigNumbers;

bool testSimple() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001}, false), 10, 0); // 1
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b00000010}, false), 15, 0); // 2

    return first.compare(second) < 0;
}

bool testDifferentExponents() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001}, false), 10, 10);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b00000010}, false), 15, 5);

    return first.compare(second) > 0;
}

bool testDifferentSizes() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001, 0b10101010}, false), 10, 0);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b00000010}, false), 15, 0);

    return first.compare(second) > 0;
}

bool testDifferentSizes2() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001, 0b10101010}, false), 10, 0);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b10101010}, false), 15, 0);

    return first.compare(second) > 0;
}

bool testDifferentSizes3() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000000, 0b10101010}, false), 10, 0);
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b10101010}, false), 15, 0);

    return first.compare(second) == 0;
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple comparison",   testSimple},
            {"Different exponents", testDifferentExponents},
            {"Different sizes",     testDifferentSizes},
            {"Different sizes 2",   testDifferentSizes2},
            {"Different sizes 3",   testDifferentSizes3}
    };

    return runTests(tests);
}