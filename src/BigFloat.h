#ifndef BIG_NUMBERS_BIGFLOAT_H
#define BIG_NUMBERS_BIGFLOAT_H

#include "BigFloatBackend.h"

#define BIG_FLOAT_PIECE_TYPE uint8_t

namespace BigNumbers {
    class BigFloat {
    private:
        BigFloatBackend<BIG_FLOAT_PIECE_TYPE> backend;
    public:
        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = true>
        explicit BigFloat(Value value): backend(BigIntBackend<BIG_FLOAT_PIECE_TYPE>(value)) {
        }

        BigFloat &operator+=(const BigFloat &addend);

        BigFloat operator+(const BigFloat &addend) const;

        BigFloat &operator++();

        BigFloat operator++(int);

        BigFloat &operator-=(const BigFloat &subtrahend);

        BigFloat operator-(const BigFloat &subtrahend) const;

        BigFloat &operator--();

        BigFloat operator--(int);

        BigFloat &operator*=(const BigFloat &multiplicand);

        BigFloat operator*(const BigFloat &multiplicand) const;

        BigFloat &operator/=(const BigFloat &divisor);

        BigFloat operator/(const BigFloat &divisor) const;

        BigFloat operator-() const;

        bool operator==(const BigFloat &other) const;

        bool operator!=(const BigFloat &other) const;

        bool operator<(const BigFloat &other) const;

        bool operator>(const BigFloat &other) const;

        bool operator<=(const BigFloat &other) const;

        bool operator>=(const BigFloat &other) const;

        friend std::ostream &operator<<(std::ostream &, const BigFloat &value);
    };
}
#endif //BIG_NUMBERS_BIGFLOAT_H
