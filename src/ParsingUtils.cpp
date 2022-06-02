#include "ParsingUtils.h"

#include <bitset>
#include <list>
#include <forward_list>

std::vector<uint8_t> decimalStringToNumbers(const std::string &source) {
    std::vector<uint8_t> transformedSource;
    transformedSource.resize(source.size());

    std::transform(source.begin(), source.end(), transformedSource.begin(), [](char in) {
        return in - '0';
    });

    return transformedSource;
}

template<class V>
std::vector<V> integralSourceToBinary(const std::string &source) {
    std::forward_list<uint8_t> binaryNumbers;

    std::vector<uint8_t> transformedSource = decimalStringToNumbers(source);

    uint8_t position = 0;
    constexpr std::size_t BIT_COUNT = 8 * sizeof(V);
    std::bitset<BIT_COUNT> buff;

    while (!transformedSource.empty()) {
        bool carry = false;
        uint8_t &current = transformedSource.back();

        if (current % 2 == 0) {
            buff.set(position, false);
        } else {
            buff.set(position, true);
            current -= 1;
        }

        ++position;
        if (position == BIT_COUNT) {
            position = 0;
            binaryNumbers.push_front(buff.to_ulong());
            buff.reset();
        }

        for (uint8_t &member: transformedSource) {
            bool oldCarry = carry;
            carry = member % 2;
            member /= 2;
            if (oldCarry) {
                member += 5;
            }
        }

        if (transformedSource[0] == 0) {
            transformedSource.erase(transformedSource.begin());
        }
    }

    if (buff.any()) {
        binaryNumbers.push_front(buff.to_ulong());
    }

    return {binaryNumbers.begin(), binaryNumbers.end()};
}

template<class V>
std::pair<std::vector<V>, uint32_t> fractionalSourceToBinary(const std::string &source,
                                                             std::size_t width, bool shift) {
    constexpr std::size_t BIT_COUNT = 8 * sizeof(V);

    std::size_t currentWidth = 0;
    std::list<V> pieces;

    std::vector<uint8_t> transformedSource = decimalStringToNumbers(source);
    uint8_t bufferIndex = 0;
    std::bitset<BIT_COUNT> buffer;
    int32_t exponent = 0;

    while (!transformedSource.empty() && currentWidth <= width) {
        bool carry = false;

        for (auto member = transformedSource.rbegin(); member < transformedSource.rend(); ++member) {
            *member = (*member) * 2 + carry;
            carry = *member >= 10;
            *member %= 10;
        }

        buffer.set(BIT_COUNT - bufferIndex - 1, carry);

        if (transformedSource.back() == 0) {
            transformedSource.pop_back();
        }

        ++bufferIndex;
        if (bufferIndex == BIT_COUNT) {
            bufferIndex = 0;

            if (currentWidth <= 1 && !carry && shift) {
                currentWidth = 0;
                --exponent;
            } else {
                ++currentWidth;
                pieces.push_back(buffer.to_ulong());
            }

            buffer.reset();
        }
    }

    if (buffer.any()) {
        pieces.push_back(buffer.to_ulong());
        ++currentWidth;
    }

    if (currentWidth > width) {
        buffer = pieces.back();
        pieces.pop_back();
        --currentWidth;

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

template std::vector<uint8_t> integralSourceToBinary(const std::string &source);

template std::pair<std::vector<uint8_t>, uint32_t> fractionalSourceToBinary(const std::string &source,
                                                                            std::size_t width, bool shift);