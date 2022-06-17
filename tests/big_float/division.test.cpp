#include "BigFloat.hpp"

#include <iostream>

#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigFloat<uint8_t> first(BigIntDebugger<uint8_t>::createFromSource({0b00000001}, 0), 4, 0); // 1
    BigFloat<uint8_t> second(BigIntDebugger<uint8_t>::createFromSource({0b00000010}, 0), 4, 0); // 2

    BigFloat<uint8_t> out = first / second;

    BigInt<uint8_t> mantissa = BigIntDebugger<uint8_t>::createFromSource({0b10000000}, 0); // 0.5
    int32_t exponent = -1;

    return testBigFloat(out, BigFloat<uint8_t>(mantissa, 4, exponent));
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