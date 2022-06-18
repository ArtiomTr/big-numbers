#include "BigFloatBackend.h"

#include <iostream>

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b00000001}, false), 4, 0); // 1
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b00000010}, false), 4, 0); // 2

    first.divide(second);

    BigIntBackend<uint8_t> mantissa({0b10000000}, false); // 0.5
    int32_t exponent = -1;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 4, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple division", testSimple},
    };

    try {
        for (auto[testName, runTest]: tests) {
            std::cout << "Test case: \"" << testName << '"' << std::endl;
            if (!runTest()) {
                return 1;
            }
        }

    } catch (...) {
        std::cout << "Unexpected exception" << std::endl;
        return 1;
    }

    return 0;
}