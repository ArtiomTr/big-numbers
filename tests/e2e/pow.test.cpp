#include <iostream>
#include <vector>

#include "BigInt.h"
#include "BigFloat.h"
#include "BigFloatMath.h"
#include "../utils.h"

using namespace BigNumbers;

bool areFloatsEqual(BigFloat first, const BigFloat &second, std::size_t precision) {
    first -= second;

    if (first < 0) {
        first = -first;
    }

    return first <= (BigFloat::epsilon(precision));
}

bool testBigInt() {
    BigFloat value = 5;

    return pow(value, 5) == BigFloat(3125);
}

bool testBigInt2() {
    BigFloat value = 60;

    return pow(value, 0) == BigFloat(1);
}

bool testBigFloat() {
    BigFloat value = 0.3;

    return areFloatsEqual(pow(value, 6), BigFloat(0.000729), 3);
}

bool testBigFloat2() {
    BigFloat value = 0.3;

    return areFloatsEqual(pow(value, 1), BigFloat(0.3), 3);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"BigInt test",     testBigInt},
            {"BigInt test 2",   testBigInt2},
            {"BigFloat test",   testBigFloat},
            {"BigFloat test 2", testBigFloat2}
    };

    return runTests(tests);
}