#include <iostream>
#include <sstream>

#include "BigFloat.h"
#include "BigFloatMath.h"
#include "../utils.h"

using namespace BigNumbers;

int main() {
    auto input = safeRelativeOpen("pi.txt");
    std::string expectedResult;
    input >> expectedResult;
    input.close();

    BigFloat::setDefaultPrecision(22);
    std::stringstream builder;

    builder << std::setprecision(99) << pi(100);
    std::string receivedResult = builder.str();

    if (receivedResult != expectedResult) {
        std::cout << "Failed to compute PI\n"
                  << "Expected: " << expectedResult << '\n'
                  << "Received: " << receivedResult << std::endl;

        return 1;
    }

    std::cout << "Test succeeded." << std::endl;

    return 0;
}