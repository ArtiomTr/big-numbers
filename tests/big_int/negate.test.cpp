#include "BigIntBackend.h"

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> one({0b00000010}, false);

    one.negate();

    return testBigInt(one, BigIntBackend<uint8_t>({0b11111110}, true));
}

bool testZero() {
    BigIntBackend<uint8_t> zero({0b00000000}, false);

    zero.negate();

    return testBigInt(zero, BigIntBackend<uint8_t>({0b00000000}, false));
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell", testSingleCell},
            {"Test zero",   testZero}
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}