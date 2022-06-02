#include "parsing_utils.h"

#include <bitset>
#include <list>
#include <forward_list>
#include <iostream>

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

template<class V>
std::pair<std::vector<V>, uint32_t> fractional_source_to_binary(const std::string &source,
                                                                std::size_t width, bool shift) {
    constexpr std::size_t BIT_COUNT = 8 * sizeof(V);

    std::size_t current_width = 0;
    std::list<V> pieces;

    std::vector<uint8_t> transformed_source = decimal_string_to_numbers(source);
    uint8_t bufferIndex = 0;
    std::bitset<BIT_COUNT> buffer;
    int32_t exponent = 0;

    while (!transformed_source.empty() && current_width <= width) {
        bool carry = false;

        for (auto member = transformed_source.rbegin(); member < transformed_source.rend(); ++member) {
            *member = (*member) * 2 + carry;
            carry = *member >= 10;
            *member %= 10;
        }

        buffer.set(BIT_COUNT - bufferIndex - 1, carry);

        if (transformed_source.back() == 0) {
            transformed_source.pop_back();
        }

        ++bufferIndex;
        if (bufferIndex == BIT_COUNT) {
            bufferIndex = 0;

            if (current_width <= 1 && !carry && shift) {
                current_width = 0;
                --exponent;
            } else {
                ++current_width;
                pieces.push_back(buffer.to_ulong());
            }

            buffer.reset();
        }
    }

    if (buffer.any()) {
        pieces.push_back(buffer.to_ulong());
        ++current_width;
    }

    if (current_width > width) {
        buffer = pieces.back();
        pieces.pop_back();
        --current_width;

        if (buffer[BIT_COUNT - 1]) {
            bool carry = true;
            for (auto it = pieces.rbegin(); it != pieces.rend(); ++it) {
                V oldValue = *it;
                *it += 1;
                if (*it > oldValue) {
                    carry = false;
                    break;
                }
            }

            if (carry) {
                pieces.push_front(0b1);
            }

            typename std::list<V>::iterator lastNonZero;
            for (auto it = pieces.begin(); it != pieces.end(); ++it) {
                if (*it != 0) {
                    lastNonZero = it;
                }
            }

            ++lastNonZero;
            pieces.erase(lastNonZero, pieces.end());
        }
    }

    return {{pieces.begin(), pieces.end()}, pieces.empty() ? 0 : exponent};
}

template std::vector<uint8_t> integral_source_to_binary(const std::string &source);

template std::pair<std::vector<uint8_t>, uint32_t> fractional_source_to_binary(const std::string &source,
                                                                               std::size_t width, bool shift);