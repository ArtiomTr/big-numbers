#include <iostream>
#include <vector>

#include "BigInt.h"
#include "BigFloatMath.h"
#include "../utils.h"

using namespace BigNumbers;

bool testZero() {
    return findNextPrime(BigFloat(0)) == BigFloat(2);
}

bool testSimple() {
    return findNextPrime(BigFloat(15)) == BigFloat(17);
}

bool testLargeValue() {
    return findNextPrime(BigFloat(1237521456)) == BigFloat(1237521469);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Test zero",        testZero},
            {"Simple test",      testSimple},
            {"Test large value", testLargeValue}
    };

    return runTests(tests);
}