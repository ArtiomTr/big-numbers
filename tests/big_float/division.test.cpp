#include "BigFloatBackend.h"

#include <iostream>

#include "../utils.h"

using namespace BigNumbers;

bool areFloatsEqual(BigFloatBackend<uint8_t> first, const BigFloatBackend<uint8_t> &second, std::size_t precision) {
    first.subtract(second);

    if (first.getMantissa().getSign()) {
        first.negate();
    }

    int comparisonResult = first.compare(BigFloatBackend<uint8_t>::epsilon(precision));

    return comparisonResult <= 0;
}

bool testSimple() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001}, false), 10, 0); // 1
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b00000010}, false), 10, 0); // 2

    first.divide(second);
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b10000000}, false), 10, -1); // 0.5

    return areFloatsEqual(first, value, 9);
}

bool testSmallFraction() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001}, false), 10, 0); // 1
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b00000000, 0b00000100}, false), 10, 1); // 1024

    first.divide(second);
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b00000000, 0b01000000}, false), 10, -2); // 0.0009765625

    return areFloatsEqual(first, value, 9);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple division",     testSimple},
            {"Test small fraction", testSmallFraction}
    };

    return runTests(tests);
}