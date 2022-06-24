#include <iostream>

#include "BigFloatBackend.h"
#include "../utils.h"

using namespace BigNumbers;

bool testSimple() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>(false, {0b01010101, 0b10010010}), 0);

    return value.toString(8, false) == "146.33203125";
}

bool testLargeValue() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>(false, {0b01010101, 0b10010010}), 10);

    return value.toString(8, false) == "176904570814779779350265856";
}

bool testSmallFraction() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>(false, {0b01010101}), -10);

    return value.toString(30, false) == "0.000000000000000000000070310352";
}

bool testNegativeValue() {
    BigIntBackend<uint8_t> mantissa(false, {0b00000111});
    mantissa.negate();
    BigFloatBackend<uint8_t> value(mantissa, -1);

    return value.toString(5, false) == "-0.02734";
}

bool testRounding() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>(false, {0b11000000}), -1);

    return value.toString(1, false) == "0.8";
}

bool testRounding2() {
    BigFloatBackend<uint8_t> value(BigIntBackend<uint8_t>(false, {0b11111000}), -1);

    return value.toString(1, false) == "1";
}

bool testSmallRounding() {
    BigIntBackend<uint8_t> mantissa(false, {0b01010101});
    mantissa.negate();
    BigFloatBackend<uint8_t> value(mantissa, -10);

    return value.toString(5, true) == "0.00000";
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Simple test",                     testSimple},
            {"Large value test",                testLargeValue},
            {"Small fraction test",             testSmallFraction},
            {"Negative value test",             testNegativeValue},
            {"Rounding test",                   testRounding},
            {"Rounding test 2",                 testRounding2},
            {"Rounding negative value to zero", testSmallRounding}
    };

    return runTests(tests);
}