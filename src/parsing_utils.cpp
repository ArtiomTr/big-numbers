#include "parsing_utils.h"

#include <bitset>
#include <forward_list>

std::vector<uint8_t> decimal_string_to_numbers(const std::string &source) {
    std::vector<uint8_t> transformed_source;
    transformed_source.resize(source.size());

    std::transform(source.begin(), source.end(), transformed_source.begin(), [](char in) {
        return in - '0';
    });

    return transformed_source;
}

template<class V>
std::vector<V> integral_source_to_binary(const std::string &source) {
    std::forward_list<uint8_t> binary_numbers;

    std::vector<uint8_t> transformed_source = decimal_string_to_numbers(source);

    uint8_t position = 0;
    constexpr std::size_t BIT_COUNT = 8 * sizeof(V);
    std::bitset<BIT_COUNT> buff;

    while (!transformed_source.empty()) {
        bool carry = false;
        uint8_t &current = transformed_source.back();

        if (current % 2 == 0) {
            buff.set(position, false);
        } else {
            buff.set(position, true);
            current -= 1;
        }

        ++position;
        if (position == BIT_COUNT) {
            position = 0;
            binary_numbers.push_front(buff.to_ulong());
            buff.reset();
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

    if (buff.any()) {
        binary_numbers.push_front(buff.to_ulong());
    }

    return {binary_numbers.begin(), binary_numbers.end()};
}

template std::vector<uint8_t> integral_source_to_binary(const std::string &source);