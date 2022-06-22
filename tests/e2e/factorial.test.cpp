#include "ParsingUtils.h"
#include "IsomorphicMath.h"
#include "BigInt.h"
#include "BigFloat.h"
#include "../utils.h"

using namespace BigNumbers;

bool testBigIntFactorial() {
    std::ifstream input = safeRelativeOpen("factorial.txt");

    bool failure = false;
    while (!input.eof() && !failure) {
        int in;
        BigInt expectedResult;

        input >> in >> expectedResult;

        BigInt receivedResult = IsomorphicMath::factorial<BigInt>(in);

        if (expectedResult != receivedResult) {
            std::cout << "Failed to compute " << in << "!\n"
                      << "Expected: " << expectedResult << '\n'
                      << "Received: " << receivedResult << std::endl;
            failure = true;
        }
    }

    input.close();

    return !failure;
}

bool testBigFloatFactorial() {
    std::ifstream input = safeRelativeOpen("factorial-float.txt");

    bool failure = false;
    while (!input.eof()) {
        int in;
        BigFloat expectedResult;

        input >> in >> expectedResult;

        BigFloat receivedResult = IsomorphicMath::factorial<BigFloat>(in);
        if (receivedResult != expectedResult) {
            std::cout << "Failed to compute " << in << "!\n"
                      << "Expected: " << expectedResult << '\n'
                      << "Received: " << receivedResult << std::endl;
            failure = true;
        }
    }

    input.close();

    return !failure;
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Test BigInt factorial",   testBigIntFactorial},
            {"Test BigFloat factorial", testBigFloatFactorial},
    };

    return runTests(tests);
}
