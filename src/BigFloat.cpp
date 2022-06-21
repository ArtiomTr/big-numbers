#include "BigFloat.h"

#include "ParsingUtils.h"

namespace BigNumbers {
    BigFloat &BigFloat::operator+=(const BigFloat &addend) {
        backend.add(addend.backend);

        return *this;
    }

    BigFloat::BigFloat() : precision(8) {

    }

    BigFloat BigFloat::epsilon(std::size_t precision) {
        BigFloat epsilon;
        epsilon.backend = BigFloatBackend<BIG_FLOAT_PIECE_TYPE>::epsilon(precision);
        return epsilon;
    }

    void BigFloat::setPrecision(std::size_t precision) {
        this->precision = precision;
    }

    BigFloat BigFloat::operator+(const BigFloat &addend) const {
        BigFloat copy = *this;
        copy += addend;
        return copy;
    }

    BigFloat &BigFloat::operator++() {
        BigFloat one(1);
        (*this) += one;

        return *this;
    }

    BigFloat BigFloat::operator++(int) {
        BigFloat copy = *this;
        ++(*this);
        return copy;
    }

    BigFloat &BigFloat::operator-=(const BigFloat &subtrahend) {
        backend.subtract(subtrahend.backend);
        return *this;
    }

    BigFloat BigFloat::operator-(const BigFloat &subtrahend) const {
        BigFloat copy = *this;
        copy -= subtrahend;
        return copy;
    }

    BigFloat &BigFloat::operator--() {
        BigFloat one(1);
        *this -= one;
        return *this;
    }

    BigFloat BigFloat::operator--(int) {
        BigFloat copy = *this;
        --(*this);
        return copy;
    }

    BigFloat &BigFloat::operator*=(const BigFloat &multiplicand) {
        backend.multiply(multiplicand.backend);
        return *this;
    }

    BigFloat BigFloat::operator*(const BigFloat &multiplicand) const {
        BigFloat copy = *this;
        copy *= multiplicand;
        return copy;
    }

    BigFloat &BigFloat::operator/=(const BigFloat &divisor) {
        backend.divide(divisor.backend, precision);
        return *this;
    }

    BigFloat BigFloat::operator/(const BigFloat &divisor) const {
        BigFloat copy = *this;
        copy /= divisor;
        return copy;
    }

    BigFloat BigFloat::operator-() const {
        BigFloat copy = *this;
        copy.backend.negate();
        return copy;
    }

    bool BigFloat::operator==(const BigFloat &other) const {
        return backend.compare(other.backend) == 0;
    }

    bool BigFloat::operator!=(const BigFloat &other) const {
        return backend.compare(other.backend) != 0;
    }

    bool BigFloat::operator<(const BigFloat &other) const {
        return backend.compare(other.backend) < 0;
    }

    bool BigFloat::operator>(const BigFloat &other) const {
        return backend.compare(other.backend) > 0;
    }

    bool BigFloat::operator<=(const BigFloat &other) const {
        return backend.compare(other.backend) <= 0;
    }

    bool BigFloat::operator>=(const BigFloat &other) const {
        return backend.compare(other.backend) >= 0;
    }

    std::ostream &operator<<(std::ostream &out, const BigFloat &value) {
        out << value.backend.toString(out.precision(), (out.flags() & std::ostream::fixed));

        return out;
    }

    std::istream &operator>>(std::istream &input, BigFloat &value) {
        std::string source;
        input >> source;

        BigFloatBackend<BIG_FLOAT_PIECE_TYPE> backend = parseBigFloat<BIG_FLOAT_PIECE_TYPE>(source, input.precision());
        value.backend = backend;

        return input;
    }
}