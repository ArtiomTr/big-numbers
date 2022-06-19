#ifndef BIG_NUMBERS_BIGINT_H
#define BIG_NUMBERS_BIGINT_H

#include <iostream>

#include "BigIntBackend.h"

#define BIG_NUMBERS_PIECE_TYPE uint8_t

namespace BigNumbers {
    class BigInt {
    private:
        friend BigInt parseBigInt(std::string source);

        BigIntBackend<BIG_NUMBERS_PIECE_TYPE> backend;

        explicit BigInt(const BigIntBackend<BIG_NUMBERS_PIECE_TYPE> &);

    public:
        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = true>
        explicit BigInt(Value value): backend(BigIntBackend<BIG_NUMBERS_PIECE_TYPE>(value)) {
        }

        BigInt() = default;

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = true>
        explicit operator Value() const {
            return (Value) backend;
        }

        BigInt &operator+=(const BigInt &addend);

        BigInt operator+(const BigInt &addend) const;

        BigInt &operator++();

        BigInt operator++(int);

        BigInt &operator-=(const BigInt &subtrahend);

        BigInt operator-(const BigInt &subtrahend) const;

        BigInt &operator--();

        BigInt operator--(int);

        BigInt &operator*=(const BigInt &multiplicand);

        BigInt operator*(const BigInt &multiplicand) const;

        BigInt &operator/=(const BigInt &divisor);

        BigInt operator/(const BigInt &divisor) const;

        BigInt &operator%=(const BigInt &divisor);

        BigInt operator%(const BigInt &divisor) const;

        BigInt operator-() const;

        bool operator==(const BigInt &other) const;

        bool operator!=(const BigInt &other) const;

        bool operator<(const BigInt &other) const;

        bool operator>(const BigInt &other) const;

        bool operator<=(const BigInt &other) const;

        bool operator>=(const BigInt &other) const;

        friend std::ostream &operator<<(std::ostream &, const BigInt &value);
    };
}

#endif //BIG_NUMBERS_BIGINT_H
