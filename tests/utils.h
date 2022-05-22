#ifndef BIG_NUMBERS_UTILS_H
#define BIG_NUMBERS_UTILS_H

#include "big_int_debugger.h"

template<class T>
bool test_big_int(const big_int<T> &received, const big_int<T> &expected) {
    auto[receivedPieces, receivedSign] = big_int_debugger<uint8_t>::debug(received);
    auto[expectedPieces, expectedSign] = big_int_debugger<uint8_t>::debug(expected);

    if (expectedSign != receivedSign) {
        std::cout << "Signs do not match: \n"
                  << "Expected: " << expected.binary_str() << '\n'
                  << "Received: " << received.binary_str() << std::endl;

        return false;
    }

    if (expectedPieces.size() != receivedPieces.size()) {
        std::cout << "Lengths do not match: \n"
                  << "Expected: " << expected.binary_str() << '\n'
                  << "Received: " << received.binary_str() << std::endl;
        return false;
    }

    for (std::size_t i = 0; i < expectedPieces.size(); ++i) {
        if (expectedPieces[i] != receivedPieces[i]) {
            std::cout << "Numbers do not match: \n"
                      << "Expected: " << expected.binary_str() << '\n'
                      << "Received: " << received.binary_str() << std::endl;
            return false;
        }
    }

    return true;
}

#endif //BIG_NUMBERS_UTILS_H
