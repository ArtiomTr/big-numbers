#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "BigIntBackend.h"

namespace BigNumbers {
    template<class T>
    class BigFloatBackend {
    private:
        int32_t exponent;
        std::size_t maxMantissaWidth;
        BigIntBackend<T> mantissa;
    public:
        explicit BigFloatBackend(std::size_t maxMantissaWidth);

        BigFloatBackend(BigIntBackend<T> mantissa, std::size_t maxMantissaWidth, int32_t exponent);

        explicit BigFloatBackend(const BigIntBackend<T> &value, std::size_t maxMantissaWidth);

        explicit operator BigIntBackend<T>() const;

        void add(BigFloatBackend<T> addend);

        void subtract(BigFloatBackend<T> subtrahend);

        void negate();

        void multiply(BigFloatBackend<T> multiplicand);

        void divide(BigFloatBackend<T> divisor);

        std::string toBinaryString() const;

        std::string toString(std::size_t maxFractionWidth) const;

        BigIntBackend<T> getMantissa() const;

        int32_t getExponent() const;
    };
}

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
