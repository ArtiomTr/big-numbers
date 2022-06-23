#include "BigInt.h"

#include "BigIntBackend.h"
#include "ParsingUtils.h"

#include "config.h"

namespace BigNumbers {
    class BigInt::Implementation {
    public:
        BigIntBackend<PieceType> backend;

        Implementation() = default;

        explicit Implementation(const BigIntBackend<PieceType> &other) : backend(other) {

        }
    };

    BigInt::BigInt() : implementation(new Implementation()) {
    }

    BigInt::BigInt(const BigInt &other) : implementation(new Implementation(*other.implementation)) {
    }

    BigInt::BigInt(unsigned char *bytes, std::size_t count) :
            implementation(new Implementation(BigIntBackend<PieceType>(bytes, count))) {
    }

    BigInt::~BigInt() {
        delete implementation;
    }

    BigInt &BigInt::operator=(const BigInt &other) {
        if (&other != this) {
            delete implementation;
            implementation = new Implementation(*other.implementation);
        }

        return *this;
    }

    BigInt &BigInt::operator+=(const BigInt &addend) {
        implementation->backend.add(addend.implementation->backend);

        return *this;
    }

    BigInt BigInt::operator+(const BigInt &addend) const {
        BigInt copy = *this;
        copy += addend;
        return copy;
    }

    BigInt &BigInt::operator++() {
        implementation->backend.add(BigIntBackend<PieceType>(1));

        return *this;
    }

    BigInt BigInt::operator++(int) {
        BigInt copy = *this;
        ++(*this);

        return copy;
    }

    BigInt &BigInt::operator-=(const BigInt &subtrahend) {
        implementation->backend.subtract(subtrahend.implementation->backend);

        return *this;
    }

    BigInt BigInt::operator-(const BigInt &subtrahend) const {
        BigInt copy = *this;
        copy -= subtrahend;
        return copy;
    }

    BigInt &BigInt::operator--() {
        implementation->backend.subtract(BigIntBackend<PieceType>(1));

        return *this;
    }

    BigInt BigInt::operator--(int) {
        BigInt copy = *this;
        --(*this);

        return copy;
    }

    BigInt &BigInt::operator*=(const BigInt &multiplicand) {
        implementation->backend.multiply(multiplicand.implementation->backend);

        return *this;
    }

    BigInt BigInt::operator*(const BigInt &multiplicand) const {
        BigInt copy = *this;
        copy *= multiplicand;
        return copy;
    }

    BigInt &BigInt::operator/=(const BigInt &divisor) {
        implementation->backend.divide(divisor.implementation->backend);

        return *this;
    }

    BigInt BigInt::operator/(const BigInt &divisor) const {
        BigInt copy = *this;
        copy /= divisor;

        return copy;
    }

    BigInt &BigInt::operator%=(const BigInt &divisor) {
        implementation->backend = implementation->backend.divide(divisor.implementation->backend);

        return *this;
    }

    BigInt BigInt::operator%(const BigInt &divisor) const {
        BigInt copy = *this;

        copy %= divisor;

        return copy;
    }

    BigInt BigInt::operator-() const {
        BigInt copy = *this;
        copy.implementation->backend.negate();
        return copy;
    }

    bool BigInt::operator==(const BigInt &other) const {
        return implementation->backend.compare(other.implementation->backend) == 0;
    }

    bool BigInt::operator!=(const BigInt &other) const {
        return implementation->backend.compare(other.implementation->backend) != 0;
    }

    bool BigInt::operator<(const BigInt &other) const {
        return implementation->backend.compare(other.implementation->backend) < 0;
    }

    bool BigInt::operator>(const BigInt &other) const {
        return implementation->backend.compare(other.implementation->backend) > 0;
    }

    bool BigInt::operator<=(const BigInt &other) const {
        return implementation->backend.compare(other.implementation->backend) <= 0;
    }

    bool BigInt::operator>=(const BigInt &other) const {
        return implementation->backend.compare(other.implementation->backend) >= 0;
    }

    std::ostream &operator<<(std::ostream &out, const BigInt &value) {
        out << value.implementation->backend.toString();
        return out;
    }

    std::istream &operator>>(std::istream &input, BigInt &value) {
        std::string source;
        input >> source;
        BigIntBackend<PieceType> backend = parseBigInt<PieceType>(source);
        value.implementation->backend = backend;

        return input;
    }

    std::pair<unsigned char *, std::size_t> BigInt::getBytes() const {
        return implementation->backend.getBytes();
    }
}
