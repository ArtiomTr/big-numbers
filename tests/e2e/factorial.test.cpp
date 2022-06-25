#include "ParsingUtils.h"
#include "BigFloatMath.h"
#include "BigInt.h"
#include "BigFloat.h"
#include "../utils.h"

using namespace BigNumbers;

bool testBigFloatFactorial() {
    std::ifstream input = safeRelativeOpen("factorial-float.txt");

    bool failure = false;
    while (!input.eof() && !failure) {
        int in;
        BigFloat expectedResult;

        input >> in >> expectedResult;

        BigFloat receivedResult = factorial(in);
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
            {"Test BigFloat factorial", testBigFloatFactorial},
    };

    return runTests(tests);
}
