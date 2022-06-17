#include "BigFloat.hpp"

#include <regex>
#include <string>
#include <sstream>

#include "IsomorphicMath.hpp"
#include "VectorUtils.h"

namespace BigNumbers {

    template<class T>
    BigFloat<T>::BigFloat(std::size_t maxMantissaWidth):
            exponent(0), maxMantissaWidth(maxMantissaWidth), mantissa(BigIntBackend<T>{}) {

    }

    template<class T>
    BigFloat<T>::BigFloat(BigIntBackend<T> mantissa, std::size_t maxMantissaWidth, int32_t exponent):
            exponent(exponent), maxMantissaWidth(maxMantissaWidth), mantissa(mantissa) {
        if (mantissa.pieces.empty()) {
            this->exponent = 0;
        }
    }

    template<class T>
    std::string BigFloat<T>::toString() const {
        std::stringstream out{};
        out << mantissa.toString() << "E" << exponent;

        return out.str();
    }

    template<class T>
    BigFloat<T> BigFloat<T>::operator+(BigFloat<T> addend) const {
        BigFloat<T> augend = *this;
        int32_t outputExponent = std::max(augend.exponent, addend.exponent);

        augend.mantissa.pieces.insert(augend.mantissa.pieces.end(),
                                      IsomorphicMath::delta(augend.exponent, outputExponent),
                                      augend.mantissa.getFillValue());
        addend.mantissa.pieces.insert(addend.mantissa.pieces.end(),
                                      IsomorphicMath::delta(addend.exponent, outputExponent),
                                      addend.mantissa.getFillValue());

        std::size_t width = std::max(augend.mantissa.pieces.size(), addend.mantissa.pieces.size());

        augend.mantissa.pieces.insert(augend.mantissa.pieces.begin(),
                                      IsomorphicMath::delta(width, augend.mantissa.pieces.size()),
                                      augend.mantissa.getFillValue());
        addend.mantissa.pieces.insert(addend.mantissa.pieces.begin(),
                                      IsomorphicMath::delta(width, addend.mantissa.pieces.size()),
                                      addend.mantissa.getFillValue());

        augend.mantissa = augend.mantissa + addend.mantissa;

        outputExponent += augend.mantissa.pieces.size() - width;

        width = augend.mantissa.pieces.size();
        augend.mantissa.normalize();
        outputExponent += width - augend.mantissa.pieces.size();
        trimFront(augend.mantissa.pieces, augend.mantissa.getFillValue());

        augend.exponent = outputExponent;

        return augend;
    }

    template<class V>
    BigFloat<V> operator-(const BigFloat<V> &minuend, const BigFloat<V> &subtrahend) {
        return minuend + (-subtrahend);
    }

    template<class V>
    BigFloat<V> operator-(const BigFloat<V> &value) {
        return BigFloat<V>(-value.mantissa, value.maxMantissaWidth, value.exponent);
    }

    inline std::size_t getFractionWidth(std::size_t totalCount) {
        return totalCount > 0 ? totalCount - 1 : 0;
    }

    template<class V>
    BigFloat<V> BigFloat<V>::operator*(const BigFloat<V> &multiplicand) {
        const BigFloat<V> &multiplier = *this;
        BigFloat<V> output(maxMantissaWidth);
        output.mantissa = multiplier.mantissa * multiplicand.mantissa;
        output.exponent = multiplier.exponent + multiplicand.exponent;

        std::size_t inputFractionWidth = getFractionWidth(multiplier.mantissa.pieces.size())
                                         + getFractionWidth(multiplicand.mantissa.pieces.size());

        std::size_t resultFractionWidth = getFractionWidth(output.mantissa.pieces.size());
        output.exponent += static_cast<int32_t>(resultFractionWidth - inputFractionWidth);

        if (output.mantissa.pieces.empty()) {
            output.exponent = 0;
        }

        if (output.mantissa.pieces.size() > maxMantissaWidth) {
            output.mantissa.pieces.erase(output.mantissa.pieces.begin() + maxMantissaWidth,
                                         output.mantissa.pieces.end());
        }

        return output;
    }

    template<class V>
    BigFloat<V> operator/(BigFloat<V> dividend, BigFloat<V> divisor) {
        auto exponentCorrection = divisor.exponent + 1;
        divisor.exponent = -1;
        dividend.exponent -= exponentCorrection;

        BigFloat<V> two((BigIntBackend<V>) 2, dividend.maxMantissaWidth, 0);

        auto factor = two - divisor;
        for (int i = 0; i < 20; ++i) {
            dividend = dividend * factor;
            divisor = divisor * factor;

            factor = two - divisor;
        }

        return dividend;
    }

    template
    class BigFloat<uint8_t>;

    template BigFloat<uint8_t> operator-(const BigFloat<uint8_t> &minuend, const BigFloat<uint8_t> &subtrahend);

    template BigFloat<uint8_t> operator-(const BigFloat<uint8_t> &value);

    template BigFloat<uint8_t> operator/(BigFloat<uint8_t> dividend, BigFloat<uint8_t> divisor);
}