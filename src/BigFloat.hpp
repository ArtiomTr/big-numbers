#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "BigInt.hpp"

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
    explicit BigFloat(BigInt<T> mantissa, int32_t exponent);

    template<class V>
    friend BigFloat<V> operator+(BigFloat<V> augend, BigFloat<V> addend);

    template<class V>
    friend BigFloat<V> operator-(const BigFloat<V> &minuend, const BigFloat<V> &subtrahend);

    template<class V>
    friend BigFloat<V> operator-(const BigFloat<V> &value);

    template<class V>
    friend BigFloat<V> operator*(const BigFloat<V> &multiplier, const BigFloat<V> &multiplicand);

    std::string toString() const;
};

template<typename T>
BigFloat<T> parseBigFloat(std::string source, std::size_t mantissaWidth);

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
