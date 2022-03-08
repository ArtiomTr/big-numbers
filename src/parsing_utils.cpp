#include "parsing_utils.h"

std::vector<uint8_t> decimal_string_to_numbers(const std::string &source) {
    std::vector<uint8_t> transformed_source;
    transformed_source.resize(source.size());

    std::transform(source.begin(), source.end(), transformed_source.begin(), [](char in) {
        return in - '0';
    });

    return transformed_source;
}

std::vector<uint8_t> integral_source_to_binary(const std::string &source) {
    std::vector<uint8_t> binary_numbers;

    std::vector<uint8_t> transformed_source = decimal_string_to_numbers(source);

    while (!transformed_source.empty()) {
        bool carry = false;
        uint8_t &current = transformed_source.back();

        if (current % 2 == 0) {
            binary_numbers.push_back(0);
        } else {
            binary_numbers.push_back(1);
            current -= 1;
        }

        for (uint8_t &member: transformed_source) {
            bool oldCarry = carry;
            carry = member % 2;
            member /= 2;
            if (oldCarry) {
                member += 5;
            }
        }

        if (transformed_source[0] == 0) {
            transformed_source.erase(transformed_source.begin());
        }
    }

    return {binary_numbers.rbegin(), binary_numbers.rend()};
}