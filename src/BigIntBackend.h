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

        BigIntBackend(std::vector<T> pieces, bool sign);

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

    private:
        static std::pair<const BigIntBackend<T> &, const BigIntBackend<T> &>
        sortBySize(const BigIntBackend<T> &first, const BigIntBackend<T> &second);
    };

    template<class T>
    template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type>
    BigIntBackend<T>::BigIntBackend(Value value): isNegative(value < (Value) 0) {
        constexpr std::size_t BIT_COUNT = sizeof(Value) * 8;
        std::bitset<BIT_COUNT> input(value);
        std::bitset<PIECE_SIZE> buffer;
        std::size_t bufferIndex = 0;

        std::size_t pieceCount = sizeof(Value) / sizeof(T);
        pieceCount += (sizeof(Value) > pieceCount * sizeof(T));
        pieces.resize(pieceCount);

        std::size_t pieceIndex = 0;
        for (std::size_t i = 0; i < BIT_COUNT; ++i) {
            buffer[bufferIndex] = input[i];
            ++bufferIndex;

            if (bufferIndex == PIECE_SIZE) {
                pieces[pieceIndex] = buffer.to_ulong();
                ++pieceIndex;
                buffer.reset();
                bufferIndex = 0;
            }
        }

        if (buffer.any()) {
            pieces.back() = buffer.to_ulong();
        } else if (pieceIndex != pieceCount) {
            pieces.back() = getFillValue();
        }

        normalize();
    }

    template<class T>
    template<class Value, typename std::enable_if<std::is_integral<Value>::value, bool>::type>
    BigIntBackend<T>::operator Value() const {
        const std::size_t outputSize = sizeof(Value);
        const std::size_t requiredPieceCount = outputSize / sizeof(T);

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
