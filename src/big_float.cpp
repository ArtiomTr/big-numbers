#include "big_float.hpp"

#include <regex>
#include <string>
#include <iostream>

#include "parsing_utils.h"

template<typename T>
big_float<T>::big_float(big_int<T> exponent, big_int<T> mantissa): exponent(exponent), mantissa(mantissa) {
}

template<typename T>
std::pair<std::vector<uint8_t>, big_int<T>>
fractional_source_to_binary(const std::string &source, std::size_t width, bool shift) {
    std::vector<uint8_t> transformed_source = decimal_string_to_numbers(source);

    std::vector<uint8_t> binary_numbers;

    std::size_t current_width = 0;
    big_int<T> exponent({0}, 0);

    while (!transformed_source.empty() && current_width <= width) {
        bool carry = false;
        ++current_width;

        for (auto member = transformed_source.rbegin(); member < transformed_source.rend(); ++member) {
            *member = (*member) * 2 + carry;
            carry = *member >= 10;
            *member %= 10;
        }

        if (current_width == 1 && !carry && shift) {
            --current_width;
            exponent = exponent + big_int<T>({1}, 0);
        } else {
            binary_numbers.push_back(carry);
        }

        if (transformed_source.back() == 0) {
            transformed_source.pop_back();
        }
    }

    // Rounding
    if (current_width > width) {
        bool carry = binary_numbers.back();
        binary_numbers.pop_back();

        if (carry) {
            for (auto member = binary_numbers.rbegin(); member < binary_numbers.rend(); ++member) {
                *member += carry;
                carry = *member > 1;
                *member %= 2;
                if (!carry) {
                    break;
                }
            }
        }
    }

    return {binary_numbers, exponent};
}

template<typename T>
std::string big_float<T>::binary_str() {
    std::stringstream out{};
    out << mantissa.binary_str() << "E" << exponent.binary_str();

    return out.str();
}

template<typename T>
big_float<T> parse_big_float(std::string source, std::size_t mantissa_width) {
    std::regex big_float_regex(R"(^-?\d+\.\d+$)");

    if (!std::regex_match(source, big_float_regex)) {
        throw std::invalid_argument("Invalid big_float format");
    }

    typedef typename std::vector<T>::size_type size_type;

    uint8_t sign = source[0] == '-';

    if (sign) {
        source.erase(source.begin());
    }

    std::string::size_type dot_position = source.find('.');

    std::vector<uint8_t> integral_part_binary = integral_source_to_binary(source.substr(0, dot_position));

    big_int<T> exponent({0}, 0);

    if (integral_part_binary.size() == 1 && integral_part_binary[0] == 0) {
        integral_part_binary.erase(integral_part_binary.begin());
    } else {
        exponent = exponent - (big_int<T>) integral_part_binary.size();
    }

    std::vector<uint8_t> fractional_part_binary;

    std::size_t total_mantissa_width = mantissa_width * big_int<T>::get_box_size();

    if (integral_part_binary.size() < total_mantissa_width) {
        std::size_t fraction_width = total_mantissa_width - integral_part_binary.size();

        bool shift = integral_part_binary.empty();

        auto out = fractional_source_to_binary<T>(source.substr(dot_position + 1, source.length()),
                                                  fraction_width, shift);

        fractional_part_binary = out.first;
        exponent = exponent + out.second;
    } else {
        throw std::logic_error("Mantissa is too small to save value without looses.");
    }

    std::vector<uint8_t> bits;

    bits.insert(bits.end(), integral_part_binary.begin(), integral_part_binary.end());
    bits.insert(bits.end(), fractional_part_binary.begin(), fractional_part_binary.end());

    std::vector<T> pieces = split_into_boxes<T>(bits);
    big_int<T> mantissa(pieces, 0);

    return big_float<T>(exponent, mantissa);
}

template
class big_float<uint8_t>;

template big_float<uint8_t> parse_big_float(std::string source, std::size_t mantissa_width);