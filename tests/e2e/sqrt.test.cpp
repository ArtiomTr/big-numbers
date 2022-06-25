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

    return first < (BigFloat::epsilon(precision));
}

bool testBigFloat() {
    BigFloat value = 25;

    return areFloatsEqual(sqrt(value), BigFloat(5), 6);
}

bool testBigFloat2() {
    BigFloat value = 60;

    return areFloatsEqual(sqrt(value), BigFloat(7.745967), 1);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"BigFloat test",   testBigFloat},
            {"BigFloat test 2", testBigFloat2}
    };

    return runTests(tests);
}