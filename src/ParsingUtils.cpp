#include "ParsingUtils.h"

#include <bitset>
#include <regex>

#include "VectorUtils.h"

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
    void integralSourceToBinary(const std::string &source, std::vector<V> &out) {
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
                out.push_back(buff.to_ulong());
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
            out.push_back(buff.to_ulong());
        }
    }

    template<class V>
    uint32_t fractionalSourceToBinary(const std::string &source, std::size_t width, std::vector<V> &output) {
        constexpr std::size_t BIT_COUNT = 8 * sizeof(V);

        std::vector<uint8_t> transformedSource = decimalStringToNumbers(source);
        uint8_t bufferIndex = 0;
        std::bitset<BIT_COUNT> buffer;
        int32_t exponent = 0;

        while (!transformedSource.empty() && output.size() <= width) {
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

                if (output.empty() && buffer.none()) {
                    --exponent;
                } else {
                    output.insert(output.begin(), buffer.to_ulong());
                }

                buffer.reset();
            }
        }

        if (buffer.any()) {
            output.insert(output.begin(), buffer.to_ulong());
        }

        if (output.size() > width) {
            buffer = std::bitset<BIT_COUNT>(output.front());
            output.erase(output.begin());

            if (buffer[BIT_COUNT - 1]) {
                bool carry = true;
                for (auto it = output.begin(); it != output.end(); ++it) {
                    V oldValue = *it;
                    *it += 1;
                    if (*it > oldValue) {
                        carry = false;
                        break;
                    }
                }

                if (carry) {
                    output.push_back(0b1);
                }

                trimBack(output, (V) 0);
            }
        }

        return output.empty() ? 0 : exponent;
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
        BigInt<T> mantissa;
        integralSourceToBinary<T>(source.substr(0, dotPosition), mantissa.pieces);

        trimBack(mantissa.pieces, (T) 0);

        int32_t exponent = std::max(mantissa.pieces.size(), static_cast<std::size_t>(1)) - 1;

        if (mantissa.pieces.size() > mantissaWidth) {
            throw std::logic_error(
                    "Too small precision: unsafe integer bound exceeded, precision is less than 1 unit.");
        }

        int32_t exponentCorrection = fractionalSourceToBinary<T>(source.substr(dotPosition + 1),
                                                                 mantissaWidth,
                                                                 mantissa.pieces);

        exponent += exponentCorrection;

        trimFront(mantissa.pieces, (T) 0);

        return BigFloat<T>(sign ? -mantissa : mantissa, mantissaWidth, exponent);
    }

    template BigInt<uint8_t> parseBigInt(std::string source);

    template BigFloat<uint8_t> parseBigFloat(std::string source, std::size_t mantissaWidth);
}
