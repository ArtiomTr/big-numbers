#ifndef BIG_NUMBERS_BIGFLOAT_H
#define BIG_NUMBERS_BIGFLOAT_H

#include "BigFloatBackend.h"

#include <sstream>

#define BIG_FLOAT_PIECE_TYPE uint8_t

namespace BigNumbers {
    class BigFloat {
    private:
        BigFloatBackend<BIG_FLOAT_PIECE_TYPE> backend;

        std::size_t precision;
    public:
        explicit BigFloat();

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        BigFloat(Value value): backend(BigIntBackend<BIG_FLOAT_PIECE_TYPE>(value)), precision(8) {
        }

        template<class Value, typename std::enable_if<std::is_floating_point<Value>::value, bool>::type = false>
        BigFloat(Value value) {
            std::stringstream builder;
            builder << value;
            builder >> *this;
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

        friend std::ostream &operator<<(std::ostream &output, const BigFloat &value);

        friend std::istream &operator>>(std::istream &input, BigFloat &value);

        void setPrecision(std::size_t precision);
    };
}
#endif //BIG_NUMBERS_BIGFLOAT_H
