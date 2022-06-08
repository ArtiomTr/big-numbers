#include "ParsingUtils.h"

#include <bitset>
#include <regex>

namespace BigNumbers {
    std::vector<uint8_t> decimalStringToNumbers(const std::string &source) {
        std::vector<uint8_t> transformedSource;
        transformedSource.resize(source.size());

        std::transform(source.begin(), source.end(), transformedSource.begin(), [](char in) {
            return in - '0';
        });

        return transformedSource;
    }

    template<class V>
    void integralSourceToBinary(const std::string &source, DoubleEndedPolynomial<V> &out) {
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
                out.pushBack(buff.to_ulong());
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
            out.pushBack(buff.to_ulong());
        }
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

    template<class T>
    BigInt<T> parseBigInt(std::string source) {
        std::regex bigIntRegex("^-?\\d+$");

        if (!std::regex_match(source, bigIntRegex)) {
            throw std::invalid_argument("Invalid BigInt format");
        }

        uint8_t sign = source[0] == '-';

        if (sign) {
            source.erase(source.begin());
        }

        BigInt<T> out;
        integralSourceToBinary<T>(source, out.pieces);

        return sign ? -out : out;
    }

    template<typename T>
    BigFloat<T> parseBigFloat(std::string source, std::size_t mantissaWidth) {
        std::regex bigFloatRegex(R"(^-?\d+\.\d+$)");

        if (!std::regex_match(source, bigFloatRegex)) {
            throw std::invalid_argument("Invalid BigFloat format");
        }

        uint8_t sign = source[0] == '-';

        if (sign) {
            source.erase(source.begin());
        }

        std::string::size_type dotPosition = source.find('.');
        std::vector<T> integralPart = integralSourceToBinary<T>(source.substr(0, dotPosition));

        std::size_t end;
        for (end = integralPart.size(); end > 0; --end) {
            if (integralPart[end - 1] != 0b0) {
                break;
            }
        }

        integralPart.erase(integralPart.begin() + end, integralPart.end());
        int32_t exponent = std::max((int) integralPart.size() - 1, 0);

        if (integralPart.size() > mantissaWidth) {
            throw std::logic_error(
                    "Too small precision: unsafe integer bound exceeded, precision is less than 1 unit.");
        }

        if (integralPart.size() < mantissaWidth) {
            auto fractionPart = fractionalSourceToBinary<T>(source.substr(dotPosition + 1),
                                                            mantissaWidth - integralPart.size(),
                                                            integralPart.empty());

            integralPart.insert(integralPart.end(), fractionPart.first.begin(), fractionPart.first.end());

            exponent += fractionPart.second;
        }

        std::reverse(integralPart.begin(), integralPart.end());
        BigInt<T> mantissa(integralPart, 0);

        return BigFloat<T>(sign ? -mantissa : mantissa, exponent);
    }
}