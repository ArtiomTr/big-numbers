#ifndef BIG_NUMBERS_BIGINT_H
#define BIG_NUMBERS_BIGINT_H

#include <istream>
#include <ostream>
#include <cstring>
#include <stdexcept>

namespace BigNumbers {
    class BigFloat;

    class BigInt {
    private:
        friend class BigFloat;

        class Implementation;

        Implementation *implementation;
    public:
        BigInt();

        BigInt(const BigInt &other);

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        BigInt(Value value);

        BigInt(unsigned char *bytes, std::size_t count);

        ~BigInt();

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        explicit operator Value() const;

        BigInt &operator=(const BigInt &other);

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

        std::pair<unsigned char *, std::size_t> getBytes() const;
    };

    template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type>
    BigInt::BigInt(Value value): BigInt(reinterpret_cast<unsigned char *>(&value), sizeof(Value)) {
    }

    template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type>
    BigInt::operator Value() const {
        Value value = 0;

        auto byteArray = getBytes();

        if (byteArray.second - 1 > sizeof(Value)) {
            throw std::logic_error("Cannot cast BigIntBackend to given type - value is too big.");
        }

        std::memcpy(reinterpret_cast<unsigned char *>(&value), byteArray.first, byteArray.second - 1);

        return value;
    }
}

#endif //BIG_NUMBERS_BIGINT_H
