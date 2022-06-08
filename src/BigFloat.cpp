#include "BigFloat.hpp"

#include <regex>
#include <string>
#include <iostream>

#include "ParsingUtils.h"
#include "IsomorphicMath.hpp"

namespace BigNumbers {

    template<class T>
    BigFloat<T>::BigFloat(BigInt<T> mantissa, int32_t exponent): exponent(exponent), mantissa(mantissa) {
        if (mantissa.getWidth() == 0) {
            this->exponent = 0;
        }
    }

    template<class T>
    std::string BigFloat<T>::toString() const {
        std::stringstream out{};
        out << mantissa.toString() << "E" << exponent;

        return out.str();
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

    template<class V>
    BigFloat<V> operator+(BigFloat<V> augend, BigFloat<V> addend) {
        int32_t exponent = std::max(augend.exponent, addend.exponent);

        augend.mantissa.pushRight(IsomorphicMath::delta(augend.exponent, exponent));
        addend.mantissa.pushRight(IsomorphicMath::delta(addend.exponent, exponent));

        std::size_t width = std::max(augend.mantissa.getWidth(), addend.mantissa.getWidth());
        augend.mantissa.padRight(width);
        addend.mantissa.padRight(width);

        BigInt<V> newMantissa = augend.mantissa + addend.mantissa;

        exponent += newMantissa.getWidth() - width;

        width = newMantissa.getWidth();
        newMantissa = newMantissa.trim();
        exponent += width - newMantissa.getWidth();
        newMantissa.trimRight();

        BigFloat<V> out(newMantissa, exponent);

        return out;
    }

    template<class V>
    BigFloat<V> operator-(const BigFloat<V> &minuend, const BigFloat<V> &subtrahend) {
        return minuend + (-subtrahend);
    }

    template<class V>
    BigFloat<V> operator-(const BigFloat<V> &value) {
        return BigFloat<V>(-value.mantissa, value.exponent);
    }

    inline std::size_t getFractionWidth(std::size_t totalCount) {
        return totalCount > 0 ? totalCount - 1 : 0;
    }

    template<class V>
    BigFloat<V> operator*(const BigFloat<V> &multiplier, const BigFloat<V> &multiplicand) {
        BigInt<V> mantissa = multiplier.mantissa * multiplicand.mantissa;
        int32_t exponent = multiplier.exponent + multiplicand.exponent;

        std::size_t inputFractionWidth = getFractionWidth(multiplier.mantissa.getWidth())
                                         + getFractionWidth(multiplicand.mantissa.getWidth());

        std::size_t resultFractionWidth = getFractionWidth(mantissa.getWidth());
        exponent += static_cast<int32_t>(resultFractionWidth - inputFractionWidth);

        return BigFloat<V>(mantissa, exponent);
    }

    template
    class BigFloat<uint8_t>;

    template BigFloat<uint8_t> parseBigFloat(std::string source, std::size_t mantissaWidth);

    template BigFloat<uint8_t> operator+(BigFloat<uint8_t> augend, BigFloat<uint8_t> addend);

    template BigFloat<uint8_t> operator-(const BigFloat<uint8_t> &minuend, const BigFloat<uint8_t> &subtrahend);

    template BigFloat<uint8_t> operator-(const BigFloat<uint8_t> &value);

    template BigFloat<uint8_t> operator*(const BigFloat<uint8_t> &multiplier, const BigFloat<uint8_t> &multiplicand);
}