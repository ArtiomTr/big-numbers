#include "BigIntBackend.h"

#include <sstream>

#include "../utils.h"

using namespace BigNumbers;

bool testSingleCell() {
    BigIntBackend<uint8_t> value({0b00000110}, false);

    std::stringstream builder;
    builder << value;

    return builder.str() == "6";
}

bool testMultipleCells() {
    BigIntBackend<uint8_t> value({0b00000000, 0b00001100}, false);

    std::stringstream builder;
    builder << value;

    return builder.str() == "3072";
}

bool testLargeValue() {
    BigIntBackend<uint8_t> value(
            {0b00100100, 0b00001100, 0b11011000, 0b00110011,
             0b11001100, 0b00010001, 0b11100011, 0b00100100,
             0b10101010, 0b00000001}, false);

    std::stringstream builder;
    builder << value;

    return builder.str() == "7860970963174088510500";
}

bool testZero() {
    BigIntBackend<uint8_t> value;

    std::stringstream builder;
    builder << value;

    return builder.str() == "0";
}

bool testNegative() {
    BigIntBackend<uint8_t> value({0b11111010}, true);

    std::stringstream builder;
    builder << value;

    return builder.str() == "-6";

}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Single cell",         testSingleCell},
            {"Test multiple cells", testMultipleCells},
            {"Test large value",    testLargeValue},
            {"Test zero",           testZero},
            {"Test negative",       testNegative}
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}