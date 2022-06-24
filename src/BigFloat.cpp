#include "BigFloat.h"

#include "BigFloatBackend.h"
#include "ParsingUtils.h"
#include "config.h"

namespace BigNumbers {
    class BigFloat::Implementation {
    public:
        std::size_t precision;
        BigFloatBackend<PieceType> backend;

        static std::size_t defaultPrecision;

        Implementation() : precision(defaultPrecision), backend() {

        }

        explicit Implementation(const BigFloatBackend<PieceType> &other) : precision(defaultPrecision), backend(other) {

        }

        explicit Implementation(const BigIntBackend<PieceType> &other) : precision(defaultPrecision), backend(other) {

        }
    };

    std::size_t BigFloat::Implementation::defaultPrecision = 8;

    BigFloat &BigFloat::operator+=(const BigFloat &addend) {
        implementation->backend.add(addend.implementation->backend);

        return *this;
    }

    BigFloat::BigFloat() : implementation(new Implementation()) {

    }

    BigFloat::~BigFloat() {
        delete implementation;
    }

    BigFloat &BigFloat::operator=(const BigFloat &other) {
        if (&other != this) {
            delete implementation;
            implementation = new Implementation(*other.implementation);
        }

        return *this;
    }

    BigFloat::BigFloat(const BigFloat &other) : implementation(new Implementation(*other.implementation)) {
    }

    BigFloat::BigFloat(const BigInt &value) : implementation(nullptr) {
        auto bytes = value.getBytes();
        *this = BigFloat(bytes.first, bytes.second);
    }

    BigFloat::BigFloat(unsigned char *bytes, std::size_t size) :
            implementation(new Implementation(BigIntBackend<PieceType>(bytes, size))) {

    }

    BigFloat BigFloat::epsilon(std::size_t precision) {
        BigFloat epsilon;
        epsilon.implementation = new Implementation(BigFloatBackend<PieceType>::epsilon(precision));
        epsilon.setPrecision(precision);
        return epsilon;
    }

    void BigFloat::setPrecision(std::size_t precision) {
        implementation->precision = precision;
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
        implementation->backend.subtract(subtrahend.implementation->backend);
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
        implementation->backend.multiply(multiplicand.implementation->backend, implementation->precision);
        return *this;
    }

    BigFloat BigFloat::operator*(const BigFloat &multiplicand) const {
        BigFloat copy = *this;
        copy *= multiplicand;
        return copy;
    }

    BigFloat &BigFloat::operator/=(const BigFloat &divisor) {
        implementation->backend.divide(divisor.implementation->backend, implementation->precision);
        return *this;
    }

    BigFloat BigFloat::operator/(const BigFloat &divisor) const {
        BigFloat copy = *this;
        copy /= divisor;
        return copy;
    }

    BigFloat BigFloat::operator-() const {
        BigFloat copy = *this;
        copy.implementation->backend.negate();
        return copy;
    }

    bool BigFloat::operator==(const BigFloat &other) const {
        return implementation->backend.compare(other.implementation->backend) == 0;
    }

    bool BigFloat::operator!=(const BigFloat &other) const {
        return implementation->backend.compare(other.implementation->backend) != 0;
    }

    bool BigFloat::operator<(const BigFloat &other) const {
        return implementation->backend.compare(other.implementation->backend) < 0;
    }

    bool BigFloat::operator>(const BigFloat &other) const {
        return implementation->backend.compare(other.implementation->backend) > 0;
    }

    bool BigFloat::operator<=(const BigFloat &other) const {
        return implementation->backend.compare(other.implementation->backend) <= 0;
    }

    bool BigFloat::operator>=(const BigFloat &other) const {
        return implementation->backend.compare(other.implementation->backend) >= 0;
    }

    std::ostream &operator<<(std::ostream &out, const BigFloat &value) {
        out << value.implementation->backend.toString(out.precision(), (out.flags() & std::ostream::fixed));

        return out;
    }

    std::istream &operator>>(std::istream &input, BigFloat &value) {
        std::string source;
        input >> source;

        BigFloatBackend<PieceType> backend = parseBigFloat<PieceType>(source, input.precision());
        delete value.implementation;
        value.implementation = new BigFloat::Implementation(backend);
        value.setPrecision(input.precision());

        return input;
    }

    BigFloat &BigFloat::operator<<(std::size_t count) {
        implementation->backend.shiftLeft(count);

        return *this;
    }

    int32_t scale05_1(BigFloat &value) {
        int32_t correction = value.implementation->backend.getExponent() + 1;

        value.implementation->backend.setExponent(-1);

        int32_t additional = 0;

        BigFloat half(0.5);
        BigFloat one(1);

        while (value < half) {
            value << 1;
            ++additional;
        }

        return correction * 8 * static_cast<int32_t>(sizeof(PieceType)) - additional;
    }

    void BigFloat::setDefaultPrecision(std::size_t precision) {
        Implementation::defaultPrecision = precision;
    }

    std::size_t BigFloat::getDefaultPrecision() {
        return Implementation::defaultPrecision;
    }
}

#undef BIG_FLOAT_PIECE_TYPE