#include <iostream>

#include "BigFloatBackend.h"
#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b01010101, 0b10010010}, false), 5, 0);

    return value.toString(8) == "146.33203125";
}

bool testLargeValue() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b01010101, 0b10010010}, false), 15, 10);

    return value.toString(8) == "176904570814779779350265856";
}

bool testSmallFraction() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b01010101}, false), 15, -10);

    return value.toString(30) == "0.000000000000000000000070310352";
}

bool testNegativeValue() {
    BigIntBackend<uint8_t> mantissa({0b00000111}, false);
    mantissa.negate();
    BigFloatBackend<uint8_t> value(mantissa, 15, -1);

    return value.toString(5) == "-0.02734";
}

bool testRounding() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b11000000}, false), 2, -1);

    return value.toString(1) == "0.8";
}

bool testRounding2() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>({0b11111000}, false), 2, -1);

    return value.toString(1) == "1";
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",         testSimple},
            {"Large value test",    testLargeValue},
            {"Small fraction test", testSmallFraction},
            {"Negative value test", testNegativeValue},
            {"Rounding test",       testRounding},
            {"Rounding test 2",     testRounding2}
    };

    return runTests(tests);
}