#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "BigIntBackend.hpp"

namespace BigNumbers {
    template<class T>
    class BigFloatDebugger;

    template<class T>
    class BigFloatBackend {
    private:
        friend BigFloatDebugger<T>;
    private:
        int32_t exponent;
        std::size_t maxMantissaWidth;
        BigIntBackend<T> mantissa;
    public:
        explicit BigFloatBackend(std::size_t maxMantissaWidth);

        explicit BigFloatBackend(BigIntBackend<T> mantissa, std::size_t maxMantissaWidth, int32_t exponent);

        BigFloatBackend<T> operator+(BigFloatBackend<T> addend) const;

        template<class V>
        friend BigFloatBackend<V> operator-(const BigFloatBackend<V> &minuend, const BigFloatBackend<V> &subtrahend);

        template<class V>
        friend BigFloatBackend<V> operator-(const BigFloatBackend<V> &value);

        BigFloatBackend<T> operator*(const BigFloatBackend<T> &multiplicand);

        template<class V>
        friend BigFloatBackend<V> operator/(BigFloatBackend<V> dividend, BigFloatBackend<V> divisor);

        std::string toString() const;
    };
}

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
