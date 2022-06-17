#ifndef BIG_NUMBERS_UTILS_H
#define BIG_NUMBERS_UTILS_H

#include <iostream>
#include <fstream>

#include "BigIntDebugger.h"
#include "BigFloatDebugger.h"

enum class BigIntComparisonResult : uint8_t {
    SIGNS_NOT_MATCH = 0,
    LENGTHS_NOT_MATCH = 1,
    NOT_EQUAL = 2,
    EQUAL,
};

// Strictly compares two BigIntBackend's.
template<class T>
BigIntComparisonResult compareBigInt(const BigNumbers::BigIntBackend<T> &first,
                                     const BigNumbers::BigIntBackend<T> &second) {
    BigNumbers::BigIntDebugger<T> firstDebugger(first);
    BigNumbers::BigIntDebugger<T> secondDebugger(second);

    if (firstDebugger.getSign() != secondDebugger.getSign()) {
        return BigIntComparisonResult::SIGNS_NOT_MATCH;
    }

    if (firstDebugger.getPieces().size() != secondDebugger.getPieces().size()) {
        return BigIntComparisonResult::LENGTHS_NOT_MATCH;
    }

    std::vector<T> firstPieces = firstDebugger.getPieces();
    std::vector<T> secondPieces = secondDebugger.getPieces();

    for (std::size_t i = 0; i < firstPieces.size(); ++i) {
        if (firstPieces[i] != secondPieces[i]) {
            return BigIntComparisonResult::NOT_EQUAL;
        }
    }

    return BigIntComparisonResult::EQUAL;
}

template<class T>
bool testBigInt(const BigNumbers::BigIntBackend<T> &received, const BigNumbers::BigIntBackend<T> &expected) {
    BigIntComparisonResult comparisonResult = compareBigInt(received, expected);

    if (comparisonResult == BigIntComparisonResult::EQUAL) {
        return true;
    }

    std::string errorMessages[] = {"Signs do not match", "Lengths do not match", "Numbers do not match"};

    std::cout << errorMessages[static_cast<int>(comparisonResult)] << ": \n"
              << "Expected: " << expected.toString() << '\n'
              << "Received: " << received.toString() << std::endl;

    return false;
}

template<class T>
bool testBigFloat(const BigNumbers::BigFloatBackend<T> &received, const BigNumbers::BigFloatBackend<T> &expected) {
    BigNumbers::BigFloatDebugger<T> receivedDebugger(received);
    BigNumbers::BigFloatDebugger<T> expectedDebugger(expected);

    if (receivedDebugger.getExponent() != expectedDebugger.getExponent()) {
        std::cout << "Exponents do not match:\n"
                  << "Expected: " << expected.toString() << '\n'
                  << "Received: " << received.toString() << std::endl;

        return false;
    }

    std::string errorMessages[] = {"Signs do not match", "Lengths do not match", "Numbers do not match"};
    BigIntComparisonResult comparisonResult = compareBigInt(receivedDebugger.getMantissa(),
                                                            expectedDebugger.getMantissa());

    if (comparisonResult != BigIntComparisonResult::EQUAL) {
        std::cout << "Mantissa do not match:\n"
                  << errorMessages[static_cast<int>(comparisonResult)] << ":\n"
                  << "Expected: " << expected.toString() << '\n'
                  << "Received: " << received.toString() << std::endl;

        return false;
    }

    return true;
}

#define safeRelativeOpen(filename) safeRelativeOpenImpl(__FILE__, filename)

std::ifstream safeRelativeOpenImpl(const std::string &current, const std::string &filename) {
    std::size_t dirnameIndex = current.find_last_of("/\\");
    std::string dirname = current.substr(0, dirnameIndex);
    std::string filePath = dirname + "/" + filename;

    std::ifstream input(filePath);

    if (input.bad()) {
        throw std::logic_error("Failed to open file \"" + filePath + "\"");
    }

    return input;
}

#endif //BIG_NUMBERS_UTILS_H
