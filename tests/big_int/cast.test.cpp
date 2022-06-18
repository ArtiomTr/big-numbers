#include <iostream>
#include <vector>

#include "BigIntBackend.h"

using namespace BigNumbers;

bool testOnePiece() {
    BigIntBackend<uint8_t> in({0b00001111}, 0);
    auto casted = (uint8_t) in;

    return casted == 15;
}

bool testMultiplePieces() {
    BigIntBackend<uint8_t> in({0b01010000, 0b00001111}, 0);

    auto casted = (uint16_t) in;

    return casted == 3920;
}

bool testZero() {
    BigIntBackend<uint8_t> in;

    auto casted = (uint8_t) in;

    return casted == 0;
}

bool testValidation() {
    BigIntBackend<uint8_t> in({0b01010000, 0b00001111, 0b00000000, 0b00000000}, 0);

    try {
        auto casted = (uint8_t) in;
    } catch (std::logic_error &error) {
        return true;
    }

    return false;
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"One piece test",       testOnePiece},
            {"Test multiple pieces", testMultiplePieces},
            {"Test zero",            testZero},
            {"Test validation",      testValidation}
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}