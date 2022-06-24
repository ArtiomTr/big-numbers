#include <iostream>
#include <sstream>

#include "BigFloat.h"
#include "IsomorphicMath.h"
#include "../utils.h"

using namespace BigNumbers;

int main() {
    auto input = safeRelativeOpen("ln.txt");

    bool failure = false;
    BigFloat::setDefaultPrecision(22);
    BigFloat ln2 = IsomorphicMath::ln(BigFloat(2), true);

    while (!input.eof() && !failure) {
        BigFloat value, expectedResult;

        input >> std::setprecision(BigFloat::getDefaultPrecision()) >> value >> expectedResult;

        BigFloat scaledValue = value;
        int32_t correction = scale05_1(scaledValue);

        BigFloat receivedResult = IsomorphicMath::ln(scaledValue);

        BigFloat correctionFloat(correction);
        receivedResult += correctionFloat * ln2;

        std::string expectedResultStr, receivedResultStr;

        std::stringstream builder;
        builder << std::setprecision(99) << expectedResult;
        builder >> expectedResultStr;
        builder.clear();
        builder << std::setprecision(99) << receivedResult;
        builder >> receivedResultStr;

        if (expectedResultStr != receivedResultStr) {
            std::cout << "Failed to compute ln(" << value << ")\n"
                      << "Expected: " << expectedResultStr << '\n'
                      << "Received: " << receivedResultStr << std::endl;
            failure = true;
        } else {
            std::cout << "Successfully computed ln(" << value << ")" << std::endl;
        }
    }

    input.close();

    if (!failure) {
        std::cout << "All tests completed successfully" << std::endl;
    }

    return failure;
}
