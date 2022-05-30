#include "big_int.hpp"
#include "../utils.h"

#include <fstream>
#include <iostream>

big_int<uint8_t> computeFactorial(int in) {
    big_int<uint8_t> result = 1;

    for (int i = 1; i <= in; ++i) {
        result = result * big_int<uint8_t>(i);
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

        big_int<uint8_t> result = parse_big_int(resultSource);

        if (computeFactorial(in) != result) {
            std::cout << "Failed to compute " << in << "!" << std::endl;
            failure = true;
        }
    }

    input.close();

    return failure ? 1 : 0;
}