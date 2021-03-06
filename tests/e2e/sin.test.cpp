#include <iostream>

#include "BigFloat.h"
#include "BigFloatMath.h"
#include "../utils.h"

using namespace BigNumbers;

int main() {
    BigFloat::setDefaultPrecision(22);

    std::ifstream input = safeRelativeOpen("sin.txt");

    bool failure = false;

    while (!input.eof() && !failure) {
        BigFloat in;
        std::string expectedString, receivedString;

        input >> std::setprecision(BigFloat::getDefaultPrecision()) >> in >> expectedString;

        BigFloat receivedResult = sin(in);

        std::stringstream builder;
        builder << std::setprecision(99) << receivedResult;
        builder >> receivedString;

        if (expectedString != receivedString) {
            std::cout << "Failed to compute sin(" << std::setprecision(99) << in << ")\n"
                      << "Expected: " << expectedString << '\n'
                      << "Received: " << receivedString << std::endl;
            failure = true;
        }
    }

    input.close();

    if (!failure) {
        std::cout << "All tests succeeded" << std::endl;
    }

    return failure;
}