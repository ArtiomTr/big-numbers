#include <iostream>
#include <vector>

#include "BigInt.h"
#include "BigFloat.h"
#include "../utils.h"
#include "IsomorphicMath.h"

using namespace BigNumbers;

bool areFloatsEqual(BigFloat first, const BigFloat &second, std::size_t precision) {
    first -= second;

    if (first < 0) {
        first = -first;
    }

    return first <= (BigFloat::epsilon(precision));
}

bool testBigInt() {
    BigInt value = 5;

    return IsomorphicMath::pow(value, 5) == BigInt(3125);
}

bool testBigInt2() {
    BigInt value = 60;

    return IsomorphicMath::pow(value, 0) == BigInt(1);
}

bool testBigFloat() {
    BigFloat value = 0.3;

    return areFloatsEqual(IsomorphicMath::pow(value, 6), BigFloat(0.000729), 3);
}

bool testBigFloat2() {
    BigFloat value = 0.3;

    return areFloatsEqual(IsomorphicMath::pow(value, 1), BigFloat(0.3), 3);
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