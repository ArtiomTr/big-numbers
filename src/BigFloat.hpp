#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "BigIntBackend.hpp"

namespace BigNumbers {
    template<class T>
    class BigFloatDebugger;

    template<class T>
    class BigFloat {
    private:
        friend BigFloatDebugger<T>;
    private:
        int32_t exponent;
        std::size_t maxMantissaWidth;
        BigIntBackend<T> mantissa;
    public:
        explicit BigFloat(std::size_t maxMantissaWidth);

        explicit BigFloat(BigIntBackend<T> mantissa, std::size_t maxMantissaWidth, int32_t exponent);

        BigFloat<T> operator+(BigFloat<T> addend) const;

        template<class V>
        friend BigFloat<V> operator-(const BigFloat<V> &minuend, const BigFloat<V> &subtrahend);

        template<class V>
        friend BigFloat<V> operator-(const BigFloat<V> &value);

        BigFloat<T> operator*(const BigFloat<T> &multiplicand);

        template<class V>
        friend BigFloat<V> operator/(BigFloat<V> dividend, BigFloat<V> divisor);

        std::string toString() const;
    };
}

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
