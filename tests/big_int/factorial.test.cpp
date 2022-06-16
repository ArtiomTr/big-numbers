#include <iostream>

#include "BigInt.hpp"
#include "ParsingUtils.h"
#include "../utils.h"

using namespace BigNumbers;

BigInt<uint8_t> computeFactorial(int in) {
    BigInt<uint8_t> result = 1;

    for (int i = 1; i <= in; ++i) {
        result = result * BigInt<uint8_t>(i);
    }

    return result;
}

int main() {
    std::ifstream input = safeRelativeOpen("factorial.txt");

    bool failure = false;

    while (!input.eof() && !failure) {
        int in;
        std::string resultSource;

        input >> in >> resultSource;

        BigInt<uint8_t> result = parseBigInt<uint8_t>(resultSource);

        if (computeFactorial(in) != result) {
            std::cout << "Failed to compute " << in << "!" << std::endl;
            failure = true;
        }
    }

    input.close();

    return failure ? 1 : 0;
}