#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "BigInt.hpp"

namespace BigNumbers {
    template<class T>
    class BigFloatDebugger;

    template<class T>
    class BigFloat {
    private:
        friend BigFloatDebugger<T>;
    private:
        int32_t exponent;
        BigInt<T> mantissa;
    public:
        explicit BigFloat();

        explicit BigFloat(BigInt<T> mantissa, int32_t exponent);

        BigFloat<T> operator+(BigFloat<T> addend) const;

        template<class V>
        friend BigFloat<V> operator-(const BigFloat<V> &minuend, const BigFloat<V> &subtrahend);

        template<class V>
        friend BigFloat<V> operator-(const BigFloat<V> &value);

        BigFloat<T> operator*(const BigFloat<T> &multiplicand);

        std::string toString() const;
    };
}

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
