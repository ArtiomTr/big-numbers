#include <iostream>

#include "BigIntBackend.h"
#include "ParsingUtils.h"
#include "../utils.h"

using namespace BigNumbers;

BigIntBackend<uint8_t> computeFactorial(int in) {
    BigIntBackend<uint8_t> result(1);

    for (int i = 1; i <= in; ++i) {
        result.multiply(BigIntBackend<uint8_t>(i));
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

        BigIntBackend<uint8_t> result = parseBigInt<uint8_t>(resultSource);

        if (computeFactorial(in).compare(result) != 0) {
            std::cout << "Failed to compute " << in << "!" << std::endl;
            failure = true;
        }
    }

    input.close();

    return failure ? 1 : 0;
}