#include <iostream>
#include <vector>

#include "DoubleEndedPolynomial.h"

using namespace BigNumbers;

bool testMiddleInsertion() {
    DoubleEndedPolynomial<uint8_t> polynomial;

    polynomial.pushFront(0);
    polynomial.pushFront(1);
    polynomial.pushFront(2);

    auto position = ++polynomial.begin();

    for (int i = 0; i < 5; ++i) {
        polynomial.insert(position, 3);
    }

    std::vector<int> result{2, 3, 3, 3, 3, 3, 1, 0};

    auto checkIt = result.begin();
    for (auto coefficient: polynomial) {
        if (coefficient != *checkIt) {
            return false;
        }
        ++checkIt;
    }

    return true;
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Test middle insertion", testMiddleInsertion},
    };

    for (auto[testName, runTest]: tests) {
        std::cout << "Test case: \"" << testName << '"' << std::endl;
        if (!runTest()) {
            return 1;
        }
    }

    return 0;
}