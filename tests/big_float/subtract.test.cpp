#include "BigFloat.hpp"
#include "../utils.h"

#include <iostream>

using namespace BigNumbers;

bool testInvert() {
    BigFloat<uint8_t> first(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 2, -1); // 0.90625
    BigFloat<uint8_t> second(BigIntDebugger<uint8_t>::createFromSource({0b11101000}, 0), 2, -1); // 0.90625

    BigFloat<uint8_t> out = first - second;

    BigIntBackend<uint8_t> mantissa; // 0
    int32_t exponent = 0;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, 2, exponent));
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