#include <iostream>
#include <vector>

#include "BigInt.h"
#include "../utils.h"
#include "IsomorphicMath.h"

using namespace BigNumbers;

bool testZero() {
    return IsomorphicMath::findNextPrime(BigInt(0)) == BigInt(2);
}

bool testSimple() {
    return IsomorphicMath::findNextPrime(BigInt(15)) == BigInt(17);
}

bool testLargeValue() {
    return IsomorphicMath::findNextPrime(BigInt(1237521456)) == BigInt(1237521469);
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