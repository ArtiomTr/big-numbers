#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <bitset>
#include <iostream>

namespace BigNumbers {
    template<class T>
    class BigIntBackend {
    private:
        bool isNegative;
        std::vector<T> pieces;

        static constexpr std::size_t PIECE_SIZE = sizeof(T) * 8;
    public:
        using SizeType = typename std::vector<T>::size_type;

        explicit BigIntBackend();

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        explicit BigIntBackend(Value value);

        BigIntBackend(unsigned char *bytes, std::size_t count);

        BigIntBackend(bool sign, std::vector<T> pieces);

        // Perform addition operation on this object and argument. Result is written to this object.
        void add(const BigIntBackend<T> &addend);

        // Perform subtraction operation on this object and argument. Result is written to this object.
        void subtract(BigIntBackend<T> subtrahend);

        // Perform multiplication operation on this object and argument. Result is written to this object.
        void multiply(const BigIntBackend<T> &multiplicand);

        // Perform division operation on this object and argument. Result is written to this object. Returns remainder.
        BigIntBackend<T> divide(BigIntBackend<T> divisor);

        // Shift current value by specified amount of bits.
        void shiftLeft(const SizeType &shiftBy);

        // Invert all bits in this object.
        void invert();

        // Get negated representation of the same value. For instance, 2 becomes -2.
        void negate();

        // Compare current object to specified argument.
        int8_t compare(BigIntBackend<T> secondOperand) const;

        template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type = false>
        explicit operator Value() const;

        std::string toBinaryString() const;

        std::string toString() const;

        T getFillValue() const;

        void normalize();

        std::vector<T> &accessPieces();

        std::vector<T> accessPieces() const;

        int32_t getSign() const;

        std::pair<unsigned char *, std::size_t> getBytes() const;
    private:
        static std::pair<const BigIntBackend<T> &, const BigIntBackend<T> &>
        sortBySize(const BigIntBackend<T> &first, const BigIntBackend<T> &second);
    };

    template<class T>
    template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type>
    BigIntBackend<T>::BigIntBackend(Value value):
            BigIntBackend(reinterpret_cast<unsigned char *>(&value), sizeof(value)) {
    }

    template<class T>
    template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type>
    BigIntBackend<T>::operator Value() const {
        const std::size_t outputSize = sizeof(Value);
        const std::size_t requiredPieceCount = std::max((std::size_t) 1, outputSize / sizeof(T));

        const BigIntBackend<T> &current = *this;

        if (current.pieces.size() > requiredPieceCount) {
            throw std::logic_error("Cannot cast BigIntBackend to given type - value is too big.");
        }

        Value castedValue = 0b0;

        std::size_t shiftSize = 0;

        for (auto piece: pieces) {
            castedValue |= (((Value)
            piece) << shiftSize);
            shiftSize += sizeof(T) * 8;
        }

        return castedValue;
    }
}

#endif //BIG_NUMBERS_BIG_INT_HPP
