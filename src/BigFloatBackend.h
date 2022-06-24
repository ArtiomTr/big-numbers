#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "BigIntBackend.h"

namespace BigNumbers {
    template<class T>
    class BigFloatBackend {
    private:
        BigIntBackend<T> mantissa;
        int32_t exponent;
    public:
        explicit BigFloatBackend();

        BigFloatBackend(BigIntBackend<T> mantissa, int32_t exponent);

        explicit BigFloatBackend(const BigIntBackend<T> &value);

        explicit operator BigIntBackend<T>() const;

        void add(BigFloatBackend<T> addend);

        void subtract(BigFloatBackend<T> subtrahend);

        void negate();

        void multiply(BigFloatBackend<T> multiplicand);

        void divide(BigFloatBackend<T> divisor, std::size_t precision);

        int compare(const BigFloatBackend<T> &other) const;

        void trim(std::size_t fractionWidth);

        static BigFloatBackend<T> epsilon(std::size_t mantissaWidth);

        std::string toBinaryString() const;

        std::string toString(std::size_t precision, bool fixed) const;

        BigIntBackend<T> getMantissa() const;

        int32_t getExponent() const;
    };
}

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
