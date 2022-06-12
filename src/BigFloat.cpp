#include "BigFloat.hpp"

#include <regex>
#include <string>
#include <sstream>

#include "IsomorphicMath.hpp"

namespace BigNumbers {

    template<class T>
    BigFloat<T>::BigFloat(): exponent(0), mantissa(BigInt<T>{}) {

    }

    template<class T>
    BigFloat<T>::BigFloat(BigInt<T> mantissa, int32_t exponent): exponent(exponent), mantissa(mantissa) {
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

        extendBack(augend.mantissa.pieces, IsomorphicMath::delta(augend.exponent, outputExponent),
                   augend.mantissa.getFillValue());
        extendBack(addend.mantissa.pieces, IsomorphicMath::delta(addend.exponent, outputExponent),
                   addend.mantissa.getFillValue());

        std::size_t width = std::max(augend.mantissa.pieces.getSize(), addend.mantissa.pieces.getSize());

        extendFront(augend.mantissa.pieces, IsomorphicMath::delta(width, augend.mantissa.pieces.getSize()),
                    augend.mantissa.getFillValue());
        extendFront(addend.mantissa.pieces, IsomorphicMath::delta(width, addend.mantissa.pieces.getSize()),
                    addend.mantissa.getFillValue());

        augend.mantissa = augend.mantissa + addend.mantissa;

        outputExponent += augend.mantissa.pieces.getSize() - width;

        width = augend.mantissa.pieces.getSize();
        augend.mantissa.normalize();
        outputExponent += width - augend.mantissa.pieces.getSize();
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
        return BigFloat<V>(-value.mantissa, value.exponent);
    }

    inline std::size_t getFractionWidth(std::size_t totalCount) {
        return totalCount > 0 ? totalCount - 1 : 0;
    }

    template<class V>
    BigFloat<V> BigFloat<V>::operator*(const BigFloat<V> &multiplicand) {
        const BigFloat<V> &multiplier = *this;
        BigFloat<V> output{};
        output.mantissa = multiplier.mantissa * multiplicand.mantissa;
        output.exponent = multiplier.exponent + multiplicand.exponent;

        std::size_t inputFractionWidth = getFractionWidth(multiplier.mantissa.pieces.getSize())
                                         + getFractionWidth(multiplicand.mantissa.pieces.getSize());

        std::size_t resultFractionWidth = getFractionWidth(output.mantissa.pieces.getSize());
        output.exponent += static_cast<int32_t>(resultFractionWidth - inputFractionWidth);

        if (output.mantissa.pieces.empty()) {
            output.exponent = 0;
        }

        return output;
    }

    template
    class BigFloat<uint8_t>;

    template BigFloat<uint8_t> operator-(const BigFloat<uint8_t> &minuend, const BigFloat<uint8_t> &subtrahend);

    template BigFloat<uint8_t> operator-(const BigFloat<uint8_t> &value);
}