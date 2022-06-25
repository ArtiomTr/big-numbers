#ifndef BIG_NUMBERS_BIGFLOAT_H
#define BIG_NUMBERS_BIGFLOAT_H

#include "BigInt.h"

#include <sstream>
#include <iomanip>
#include <limits>

#include <iostream>

namespace BigNumbers {
    class BigFloat {
    private:
        class Implementation;

        Implementation *implementation;
    public:
        BigFloat();

        BigFloat(const BigFloat &other);

        explicit BigFloat(const BigInt &value);

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        BigFloat(Value value): BigFloat(reinterpret_cast<unsigned char *>(&value), sizeof(value)) {
        }

        template<class Value, typename std::enable_if<std::is_floating_point<Value>::value, bool>::type = false>
        BigFloat(Value value): implementation(nullptr) {
            std::stringstream builder;
            builder << std::fixed << std::setprecision(getDecimalPrecision()) << value;
            builder >> *this;
        }

        BigFloat(unsigned char *bytes, std::size_t size);

        ~BigFloat();

        explicit operator BigInt() const;

        BigFloat &operator=(const BigFloat &other);

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

        BigFloat &operator<<(std::size_t count);

        bool operator==(const BigFloat &other) const;

        bool operator!=(const BigFloat &other) const;

        bool operator<(const BigFloat &other) const;

        bool operator>(const BigFloat &other) const;

        bool operator<=(const BigFloat &other) const;

        bool operator>=(const BigFloat &other) const;

        friend std::ostream &operator<<(std::ostream &output, const BigFloat &value);

        friend std::istream &operator>>(std::istream &input, BigFloat &value);

        void setPrecision(std::size_t precision);

        static void setDefaultPrecision(std::size_t precision);

        std::size_t getPrecision() const;

        static std::size_t getDefaultPrecision();

        static BigFloat epsilon(std::size_t precision);

        int getDecimalPrecision();

        friend int32_t scale05_1(BigFloat &value);
    };
}

#endif //BIG_NUMBERS_BIGFLOAT_H
