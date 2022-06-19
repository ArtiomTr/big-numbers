#include "BigInt.h"

namespace BigNumbers {
    BigInt::BigInt(const BigIntBackend<BIG_NUMBERS_PIECE_TYPE> &backend) : backend(backend) {

    }

    BigInt &BigInt::operator+=(const BigInt &addend) {
        backend.add(addend.backend);

        return *this;
    }

    BigInt BigInt::operator+(const BigInt &addend) const {
        BigInt copy = *this;
        copy += addend;
        return copy;
    }

    BigInt &BigInt::operator++() {
        backend.add(BigIntBackend<BIG_NUMBERS_PIECE_TYPE>(1));

        return *this;
    }

    BigInt BigInt::operator++(int) {
        BigInt copy = *this;
        ++(*this);

        return copy;
    }

    BigInt &BigInt::operator-=(const BigInt &subtrahend) {
        backend.subtract(subtrahend.backend);

        return *this;
    }

    BigInt BigInt::operator-(const BigInt &subtrahend) const {
        BigInt copy = *this;
        copy -= subtrahend;
        return copy;
    }

    BigInt &BigInt::operator--() {
        backend.subtract(BigIntBackend<BIG_NUMBERS_PIECE_TYPE>(1));

        return *this;
    }

    BigInt BigInt::operator--(int) {
        BigInt copy = *this;
        --(*this);

        return copy;
    }

    BigInt &BigInt::operator*=(const BigInt &multiplicand) {
        backend.multiply(multiplicand.backend);

        return *this;
    }

    BigInt BigInt::operator*(const BigInt &multiplicand) const {
        BigInt copy = *this;
        copy *= multiplicand;
        return copy;
    }

    BigInt &BigInt::operator/=(const BigInt &divisor) {
        backend.divide(divisor.backend);

        return *this;
    }

    BigInt BigInt::operator/(const BigInt &divisor) const {
        BigInt copy = *this;
        copy /= divisor;

        return copy;
    }

    BigInt &BigInt::operator%=(const BigInt &divisor) {
        backend = backend.divide(divisor.backend);

        return *this;
    }

    BigInt BigInt::operator%(const BigInt &divisor) const {
        BigInt copy = *this;

        copy %= divisor;

        return copy;
    }

    BigInt BigInt::operator-() const {
        BigInt copy = *this;
        copy.backend.negate();
        return copy;
    }

    bool BigInt::operator==(const BigInt &other) const {
        return backend.compare(other.backend) == 0;
    }

    bool BigInt::operator!=(const BigInt &other) const {
        return backend.compare(other.backend) != 0;
    }

    bool BigInt::operator<(const BigInt &other) const {
        return backend.compare(other.backend) < 0;
    }

    bool BigInt::operator>(const BigInt &other) const {
        return backend.compare(other.backend) > 0;
    }

    bool BigInt::operator<=(const BigInt &other) const {
        return backend.compare(other.backend) <= 0;
    }

    bool BigInt::operator>=(const BigInt &other) const {
        return backend.compare(other.backend) >= 0;
    }

    std::ostream &operator<<(std::ostream &out, const BigInt &value) {
        out << value.backend.toString();
        return out;
    }
}