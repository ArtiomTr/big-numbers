#ifndef BIG_NUMBERS_UTILS_H
#define BIG_NUMBERS_UTILS_H

#include <iostream>
#include <fstream>

#include "big_int_debugger.h"
#include "big_float_debugger.h"

enum class big_int_comparison_result : uint8_t {
    SIGNS_NOT_MATCH = 0,
    LENGTHS_NOT_MATCH = 1,
    NOT_EQUAL = 2,
    EQUAL,
};

// Strictly compares two big_int's.
template<class T>
big_int_comparison_result compare_big_int(const big_int<T> &first, const big_int<T> &second) {
    big_int_debugger<T> first_debugger(first);
    big_int_debugger<T> second_debugger(second);

    if (first_debugger.get_sign() != second_debugger.get_sign()) {
        return big_int_comparison_result::SIGNS_NOT_MATCH;
    }

    if (first_debugger.get_pieces().size() != second_debugger.get_pieces().size()) {
        return big_int_comparison_result::LENGTHS_NOT_MATCH;
    }

    std::vector<T> first_pieces = first_debugger.get_pieces();
    std::vector<T> second_pieces = second_debugger.get_pieces();

    for (std::size_t i = 0; i < first_pieces.size(); ++i) {
        if (first_pieces[i] != second_pieces[i]) {
            return big_int_comparison_result::NOT_EQUAL;
        }
    }

    return big_int_comparison_result::EQUAL;
}

template<class T>
bool test_big_int(const big_int<T> &received, const big_int<T> &expected) {
    big_int_comparison_result compare_result = compare_big_int(received, expected);

    if (compare_result == big_int_comparison_result::EQUAL) {
        return true;
    }

    std::string error_messages[] = {"Signs do not match", "Lengths do not match", "Numbers do not match"};

    std::cout << error_messages[static_cast<int>(compare_result)] << ": \n"
              << "Expected: " << expected.binary_str() << '\n'
              << "Received: " << received.binary_str() << std::endl;

    return false;
}

template<class T>
bool test_big_float(const big_float<T> &received, const big_float<T> &expected) {
    big_float_debugger<T> received_debugger(received);
    big_float_debugger<T> expected_debugger(expected);

    if (received_debugger.get_exponent() != expected_debugger.get_exponent()) {
        std::cout << "Exponents do not match:\n"
                  << "Expected: " << expected_debugger.get_exponent() << '\n'
                  << "Received: " << received_debugger.get_exponent() << std::endl;

        return false;
    }

    std::string error_messages[] = {"Signs do not match", "Lengths do not match", "Numbers do not match"};
    big_int_comparison_result compare_result
            = compare_big_int(received_debugger.get_mantissa(), expected_debugger.get_mantissa());

    if (compare_result != big_int_comparison_result::EQUAL) {
        std::cout << "Mantissa do not match:\n"
                  << error_messages[static_cast<int>(compare_result)] << ":\n"
                  << "Expected: " << expected.binary_str() << '\n'
                  << "Received: " << received.binary_str() << std::endl;

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
