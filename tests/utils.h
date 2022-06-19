#ifndef BIG_NUMBERS_UTILS_H
#define BIG_NUMBERS_UTILS_H

#include <iostream>
#include <fstream>

#include "BigIntBackend.h"
#include "BigFloatBackend.h"

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
    if (first.getSign() != second.getSign()) {
        return BigIntComparisonResult::SIGNS_NOT_MATCH;
    }

    if (first.accessPieces().size() != second.accessPieces().size()) {
        return BigIntComparisonResult::LENGTHS_NOT_MATCH;
    }

    std::vector<T> firstPieces = first.accessPieces();
    std::vector<T> secondPieces = second.accessPieces();

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
              << "Expected: " << expected.toBinaryString() << '\n'
              << "Received: " << received.toBinaryString() << std::endl;

    return false;
}

template<class T>
bool testBigFloat(const BigNumbers::BigFloatBackend<T> &received, const BigNumbers::BigFloatBackend<T> &expected) {
    if (received.getExponent() != expected.getExponent()) {
        std::cout << "Exponents do not match:\n"
                  << "Expected: " << expected.toBinaryString() << '\n'
                  << "Received: " << received.toBinaryString() << std::endl;

        return false;
    }

    std::string errorMessages[] = {"Signs do not match", "Lengths do not match", "Numbers do not match"};
    BigIntComparisonResult comparisonResult = compareBigInt(received.getMantissa(),
                                                            expected.getMantissa());

    if (comparisonResult != BigIntComparisonResult::EQUAL) {
        std::cout << "Mantissa do not match:\n"
                  << errorMessages[static_cast<int>(comparisonResult)] << ":\n"
                  << "Expected: " << expected.toBinaryString() << '\n'
                  << "Received: " << received.toBinaryString() << std::endl;

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

int runTests(const std::vector<std::pair<std::string, bool (*)()>> &tests) {
    try {
        for (const auto &entry: tests) {
            std::string testName = entry.first;
            auto runTest = entry.second;

            std::cout << "Test case: \"" << testName << '"' << std::endl;
            if (!runTest()) {
                return 1;
            }
        }
    } catch (...) {
        std::cout << "Unexpected exception occurred" << std::endl;
        return 1;
    }

    return 0;
}

#endif //BIG_NUMBERS_UTILS_H
