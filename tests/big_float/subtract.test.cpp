#include "BigFloatBackend.h"
#include "../utils.h"

#include <iostream>

using namespace BigNumbers;

bool testInvert() {
    BigFloatBackend<uint8_t> first(BigIntBackend<uint8_t>({0b11101000}, false), 2, -1); // 0.90625
    BigFloatBackend<uint8_t> second(BigIntBackend<uint8_t>({0b11101000}, false), 2, -1); // 0.90625

    first.subtract(second);

    BigIntBackend<uint8_t> mantissa; // 0
    int32_t exponent = 0;

    return testBigFloat(first, BigFloatBackend<uint8_t>(mantissa, 2, exponent));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Should invert number and add", testInvert},
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