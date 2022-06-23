#include "ParsingUtils.h"

#include <bitset>
#include <regex>

#include "VectorUtils.h"
#include "config.h"

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
    uint32_t fractionalSourceToBinary(const std::string &source, std::size_t precision, std::vector<V> &output) {
        std::size_t inputWidth = output.size();
        constexpr std::size_t BIT_COUNT = 8 * sizeof(V);

        std::vector<uint8_t> transformedSource = decimalStringToNumbers(source);
        uint8_t bufferIndex = 0;
        std::bitset<BIT_COUNT> buffer;
        int32_t exponent = 0;

        if (output.empty()) {
            --exponent;
        }

        std::size_t currentFractionDigit = 0;
        while (!transformedSource.empty() && currentFractionDigit <= precision) {
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
                    ++currentFractionDigit;
                    output.insert(output.begin(), buffer.to_ulong());
                }

                buffer.reset();
            }
        }

        if (buffer.any()) {
            output.insert(output.begin(), buffer.to_ulong());
        }

        if (output.size() > inputWidth + precision) {
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
                    ++exponent;
                }

                trimBack(output, (V) 0);
            }
        }

        return output.empty() ? 0 : exponent;
    }

    template<class T>
    BigIntBackend<T> parseBigInt(std::string source) {
        std::regex bigIntRegex("^-?\\d+$");

        if (!std::regex_match(source, bigIntRegex)) {
            throw std::invalid_argument("Invalid BigIntBackend format");
        }

        uint8_t sign = source[0] == '-';

        if (sign) {
            source.erase(source.begin());
        }

        BigIntBackend<T> out;
        integralSourceToBinary<T>(source, out.accessPieces());

        if (sign) {
            out.negate();
        }

        return out;
    }

    template<typename T>
    BigFloatBackend<T> parseBigFloat(std::string source, std::size_t precision) {
        std::regex bigFloatRegex(R"(^-?\d+\.\d+$)");

        if (!std::regex_match(source, bigFloatRegex)) {
            throw std::invalid_argument("Invalid BigFloatBackend format");
        }

        uint8_t sign = source[0] == '-';

        if (sign) {
            source.erase(source.begin());
        }

        std::string::size_type dotPosition = source.find('.');
        BigIntBackend<T> mantissa;
        integralSourceToBinary<T>(source.substr(0, dotPosition), mantissa.accessPieces());

        trimBack(mantissa.accessPieces(), (T) 0);

        int32_t exponent = std::max(mantissa.accessPieces().size(), static_cast<std::size_t>(1)) - 1;

        int32_t exponentCorrection = fractionalSourceToBinary<T>(source.substr(dotPosition + 1),
                                                                 precision,
                                                                 mantissa.accessPieces());

        exponent += exponentCorrection;

        trimFront(mantissa.accessPieces(), (T) 0);

        if (sign) {
            mantissa.negate();
        }

        return BigFloatBackend<T>(mantissa, exponent);
    }

    // Required for testing
    template BigIntBackend<uint8_t> parseBigInt(std::string source);

    // Required for final result
    template BigIntBackend<PieceType> parseBigInt(std::string source);

    // Required for testing
    template BigFloatBackend<uint8_t> parseBigFloat(std::string source, std::size_t mantissaWidth);

    //Required for final result
    template BigFloatBackend<PieceType> parseBigFloat(std::string source, std::size_t mantissaWidth);
}
