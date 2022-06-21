#ifndef BIG_NUMBERS_BIGINT_H
#define BIG_NUMBERS_BIGINT_H

#include <istream>
#include <ostream>

#include "BigIntBackend.h"

#define BIG_INT_PIECE_TYPE uint8_t

namespace BigNumbers {
    class BigInt {
    private:
        BigIntBackend<BIG_INT_PIECE_TYPE> backend;
    public:
        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        BigInt(Value value): backend(BigIntBackend<BIG_INT_PIECE_TYPE>(value)) {
        }

        BigInt() = default;

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
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

        friend std::ostream &operator<<(std::ostream &output, const BigInt &value);

        friend std::istream &operator>>(std::istream &input, BigInt &value);
    };
}

#endif //BIG_NUMBERS_BIGINT_H
