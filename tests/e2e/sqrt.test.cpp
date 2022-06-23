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

    return first < (BigFloat::epsilon(precision));
}

bool testBigInt() {
    BigInt value = 25;

    return IsomorphicMath::sqrt(value, BigInt(0)) == BigInt(5);
}

bool testBigInt2() {
    BigInt value = 60;

    return IsomorphicMath::sqrt(value, BigInt(0)) == BigInt(7);
}

bool testBigFloat() {
    BigFloat value = 25;

    return areFloatsEqual(IsomorphicMath::sqrt(value, BigFloat::epsilon(6)), BigFloat(5), 6);
}

bool testBigFloat2() {
    BigFloat value = 60;

    return areFloatsEqual(IsomorphicMath::sqrt(value, BigFloat::epsilon(2)), BigFloat(7.745967), 1);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"BigInt test", testBigInt},
            {"BigInt test 2", testBigInt2},
            {"BigFloat test", testBigFloat},
            {"BigFloat test 2", testBigFloat2}
    };

    return runTests(tests);
}