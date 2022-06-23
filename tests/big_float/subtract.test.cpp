#include "BigFloatBackend.h"
#include "../utils.h"

#include <iostream>

using namespace BigNumbers;

bool testInvert() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>(false, {0b11101000}), -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>(false, {0b11101000}), -1); // 0.90625

    first.subtract(second);

    BigIntBackend<uint8_t> mantissa; // 0
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Should invert number and add", testInvert},
    };

    return runTests(tests);
}