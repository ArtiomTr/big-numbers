#include "BigFloatBackend.h"

#include <string>
#include <sstream>

#include "IsomorphicMath.h"
#include "VectorUtils.h"

namespace BigNumbers {
    template<class T>
    BigFloatBackend<T>::BigFloatBackend(std::size_t maxMantissaWidth):
            exponent(0), maxMantissaWidth(maxMantissaWidth), mantissa(BigIntBackend<T>{}) {

    }

    template<class T>
    BigFloatBackend<T>::BigFloatBackend(BigIntBackend<T> mantissa, std::size_t maxMantissaWidth, int32_t exponent):
            exponent(exponent), maxMantissaWidth(maxMantissaWidth), mantissa(mantissa) {
        if (mantissa.accessPieces().empty()) {
            this->exponent = 0;
        }
    }

    template<class T>
    std::string BigFloatBackend<T>::toBinaryString() const {
        std::stringstream out{};
        out << mantissa.toBinaryString() << "E" << exponent;

        return out.str();
    }

    template<class T>
    void BigFloatBackend<T>::add(BigFloatBackend<T> addend) {
        int32_t outputExponent = std::max(exponent, addend.exponent);

        bool isMantissaNegative = mantissa.getSign();
        bool isAddendNegative = addend.mantissa.getSign();

        if (isMantissaNegative) {
            mantissa.negate();
        }

        if (isAddendNegative) {
            addend.mantissa.negate();
        }

        extendBack(mantissa.accessPieces(), (T) 0, IsomorphicMath::delta(exponent, outputExponent));
        extendBack(addend.mantissa.accessPieces(), (T) 0, IsomorphicMath::delta(addend.exponent, outputExponent));

        std::size_t width = std::max(mantissa.accessPieces().size(), addend.mantissa.accessPieces().size());

        extendFront(mantissa.accessPieces(), (T) 0, IsomorphicMath::delta(width, mantissa.accessPieces().size()));
        extendFront(addend.mantissa.accessPieces(), (T) 0,
                    IsomorphicMath::delta(width, addend.mantissa.accessPieces().size()));

        if (isMantissaNegative) {
            mantissa.negate();
        }

        if (isAddendNegative) {
            addend.mantissa.negate();
        }

        mantissa.add(addend.mantissa);

        outputExponent += mantissa.accessPieces().size() - width;

        width = mantissa.accessPieces().size();
        mantissa.normalize();
        outputExponent -= width - mantissa.accessPieces().size();
        trimFront(mantissa.accessPieces(), mantissa.getFillValue());

        if (mantissa.accessPieces().empty()) {
            exponent = 0;
        } else {
            exponent = outputExponent;
        }
    }

    template<class T>
    void BigFloatBackend<T>::subtract(BigFloatBackend<T> subtrahend) {
        subtrahend.negate();
        add(subtrahend);
    }

    template<class T>
    void BigFloatBackend<T>::negate() {
        mantissa.negate();
    }

    inline std::size_t getFractionWidth(std::size_t totalCount) {
        return totalCount > 0 ? totalCount - 1 : 0;
    }

    template<class T>
    void BigFloatBackend<T>::multiply(BigFloatBackend<T> multiplicand) {
        std::size_t inputFractionWidth = getFractionWidth(mantissa.accessPieces().size())
                                         + getFractionWidth(multiplicand.mantissa.accessPieces().size());

        mantissa.multiply(multiplicand.mantissa);
        exponent += multiplicand.exponent;

        std::size_t resultFractionWidth = getFractionWidth(mantissa.accessPieces().size());
        exponent += static_cast<int32_t>(resultFractionWidth - inputFractionWidth);

        if (mantissa.accessPieces().empty()) {
            exponent = 0;
        }
    }

    template<class T>
    void BigFloatBackend<T>::divide(BigFloatBackend<T> divisor) {
        constexpr std::size_t MAX_ITER_COUNT = 30;

        auto exponentCorrection = divisor.exponent + 1;
        divisor.exponent = -1;
        exponent -= exponentCorrection;

        BigFloatBackend<T> two((BigIntBackend<T>) 2, divisor.maxMantissaWidth, 0);

        auto factor = two;
        factor.subtract(divisor);

        for (std::size_t iter = 0; iter < MAX_ITER_COUNT; ++iter) {

            multiply(factor);
            if (mantissa.accessPieces().size() > maxMantissaWidth) {
                mantissa.accessPieces().erase(mantissa.accessPieces().begin(),
                                              mantissa.accessPieces().end() - maxMantissaWidth);
            }

            divisor.multiply(factor);
            if (divisor.mantissa.accessPieces().size() > divisor.maxMantissaWidth) {
                divisor.mantissa.accessPieces().erase(divisor.mantissa.accessPieces().begin(),
                                                      divisor.mantissa.accessPieces().end() - divisor.maxMantissaWidth);
            }

            factor = two;
            factor.subtract(divisor);
        }
    }

    template<class T>
    BigFloatBackend<T>::BigFloatBackend(const BigIntBackend<T> &value, std::size_t maxMantissaWidth):
            mantissa(value), maxMantissaWidth(maxMantissaWidth), exponent(value.accessPieces().size()) {
        if (exponent > 0) {
            --exponent;
        }
    }

    template<class T>
    BigFloatBackend<T>::operator BigIntBackend<T>() const {
        BigIntBackend<T> integralPart;

        if (exponent >= 0) {
            integralPart = mantissa;
            std::size_t desiredWidth = exponent + 1;
            if (integralPart.accessPieces().size() >= desiredWidth) {
                integralPart.accessPieces().erase(integralPart.accessPieces().begin(),
                                                  integralPart.accessPieces().end() - desiredWidth);
            } else {
                if (mantissa.getSign()) {
                    integralPart.negate();
                }

                integralPart.accessPieces().insert(integralPart.accessPieces().begin(),
                                                   desiredWidth - integralPart.accessPieces().size(),
                                                   0);

                if (mantissa.getSign()) {
                    integralPart.negate();
                }
            }
        }

        return integralPart;
    }

    template<class T>
    std::string BigFloatBackend<T>::toString(std::size_t maxFractionWidth) const {
        std::string output;

        auto integralPart = static_cast<BigIntBackend<T>>(*this);

        if (integralPart.getSign()) {
            integralPart.negate();
        }

        BigFloatBackend<T> fractionalPart = *this;
        if (fractionalPart.mantissa.getSign()) {
            fractionalPart.mantissa.negate();
        }

        BigFloatBackend<T> integralAsFloat(integralPart, maxMantissaWidth);
        fractionalPart.subtract(integralAsFloat);

        BigFloatBackend<T> ten(BigIntBackend<T>(10), maxMantissaWidth);
        for (std::size_t i = 0; i <= maxFractionWidth; ++i) {
            fractionalPart.multiply(ten);
        }

        auto fractionalPartValue = (BigIntBackend<T>) fractionalPart;
        auto remainder = fractionalPartValue.divide(BigIntBackend<T>(10));

        std::string tempFractionString = fractionalPartValue.toString();
        if (remainder.compare(BigIntBackend<T>(5)) >= 0) {
            fractionalPartValue.add(BigIntBackend<T>(1));
        }
        std::string fractionString = fractionalPartValue.toString();

        if (fractionString.length() > tempFractionString.length()) {
            fractionString.erase(fractionString.begin());
            integralPart.add(BigIntBackend<T>(1));
        }

        extendFront(fractionString, '0', (int) maxFractionWidth - (int) fractionString.length());

        trimBack(fractionString, '0');

        if (mantissa.getSign()) {
            output += '-';
        }

        output += integralPart.toString();
        if (!fractionString.empty()) {
            output += '.';
            output += fractionString;
        }

        return output;
    }

    template<class T>
    int BigFloatBackend<T>::compare(const BigFloatBackend<T> &other) {
        if (mantissa.getSign() != other.mantissa.getSign()) {
            if (mantissa.getSign() > other.mantissa.getSign()) {
                return -1;
            }

            return 1;
        }

        if (exponent != other.exponent) {
            if (exponent > other.exponent) {
                return 1;
            }

            return -1;
        }

        BigIntBackend<T> firstMantissa = mantissa;
        BigIntBackend<T> secondMantissa = other.mantissa;

        if (firstMantissa.getSign()) {
            firstMantissa.negate();
            secondMantissa.negate();
        }

        std::size_t maxWidth = std::max(firstMantissa.accessPieces().size(), secondMantissa.accessPieces().size());
        extendFront(firstMantissa.accessPieces(), 0,
                    IsomorphicMath::delta(maxWidth, firstMantissa.accessPieces().size()));
        extendFront(secondMantissa.accessPieces(), 0,
                    IsomorphicMath::delta(maxWidth, secondMantissa.accessPieces().size()));

        return firstMantissa.compare(secondMantissa);
    }

    template<class T>
    BigIntBackend<T> BigFloatBackend<T>::getMantissa() const {
        return mantissa;
    }

    template<class T>
    int32_t BigFloatBackend<T>::getExponent() const {
        return exponent;
    }

    template
    class BigFloatBackend<uint8_t>;
}