#include <iostream>
#include <vector>

#include "BigInt.h"
#include "BigFloat.h"
#include "../utils.h"
#include "IsomorphicMath.h"

using namespace BigNumbers;

bool testBigInt() {
    BigInt value = 25;

    return IsomorphicMath::sqrt(value, BigInt(0)) == BigInt(5);
}

bool testBigInt2() {
    BigInt value = 60;

    return IsomorphicMath::sqrt(value, BigInt(0)) == BigInt(7);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"BigInt test",   testBigInt},
            {"BigInt test 2", testBigInt2}
    };

    return runTests(tests);
}