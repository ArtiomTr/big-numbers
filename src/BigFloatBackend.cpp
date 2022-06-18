#include "BigFloatBackend.h"

#include <regex>
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
    std::string BigFloatBackend<T>::toString() const {
        std::stringstream out{};
        out << mantissa.toString() << "E" << exponent;

        return out.str();
    }

    template<class T>
    void BigFloatBackend<T>::add(BigFloatBackend<T> addend) {
        int32_t outputExponent = std::max(exponent, addend.exponent);

        mantissa.accessPieces().insert(mantissa.accessPieces().end(),
                                       IsomorphicMath::delta(exponent, outputExponent),
                                       mantissa.getFillValue());
        addend.mantissa.accessPieces().insert(addend.mantissa.accessPieces().end(),
                                              IsomorphicMath::delta(addend.exponent, outputExponent),
                                              addend.mantissa.getFillValue());

        std::size_t width = std::max(mantissa.accessPieces().size(), addend.mantissa.accessPieces().size());

        mantissa.accessPieces().insert(mantissa.accessPieces().begin(),
                                       IsomorphicMath::delta(width, mantissa.accessPieces().size()),
                                       mantissa.getFillValue());
        addend.mantissa.accessPieces().insert(addend.mantissa.accessPieces().begin(),
                                              IsomorphicMath::delta(width, addend.mantissa.accessPieces().size()),
                                              addend.mantissa.getFillValue());

        mantissa.add(addend.mantissa);

        outputExponent += mantissa.accessPieces().size() - width;

        width = mantissa.accessPieces().size();
        mantissa.normalize();
        outputExponent += width - mantissa.accessPieces().size();
        trimFront(mantissa.accessPieces(), mantissa.getFillValue());

        exponent = outputExponent;
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

        if (mantissa.accessPieces().size() > maxMantissaWidth) {
            mantissa.accessPieces().erase(mantissa.accessPieces().begin() + maxMantissaWidth,
                                          mantissa.accessPieces().end());
        }
    }

    template<class T>
    void BigFloatBackend<T>::divide(BigFloatBackend<T> divisor) {
        auto exponentCorrection = divisor.exponent + 1;
        divisor.exponent = -1;
        exponent -= exponentCorrection;

        BigFloatBackend<T> two((BigIntBackend<T>) 2, maxMantissaWidth, 0);

        auto factor = two;
        factor.subtract(divisor);
        for (int i = 0; i < 20; ++i) {
            multiply(factor);
            divisor.multiply(factor);

            factor = two;
            factor.subtract(divisor);
        }
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