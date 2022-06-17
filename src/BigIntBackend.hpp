#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <bitset>
#include <iostream>

namespace BigNumbers {
    template<class T>
    class BigIntDebugger;

    template<class T>
    class BigFloat;

    template<class T = uint8_t>
    class BigIntBackend {
        // Wait, is that spaghetti code? Nah, it's just very friendly class :p
        friend class BigIntDebugger<T>;

        friend class BigFloat<T>;

        template<class V>
        friend BigIntBackend<V> parseBigInt(std::string);

        template<class V>
        friend BigFloat<V> parseBigFloat(std::string, std::size_t);

    private:
        uint8_t sign;
        std::vector<T> pieces;

        static constexpr std::size_t PIECE_SIZE = sizeof(T) * 8;
    public:
        using SizeType = typename std::vector<T>::size_type;

        explicit BigIntBackend();

        template<class Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
        BigIntBackend(Value value): sign(value < 0) {
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

        template<class V>
        friend BigIntBackend<V> operator+(const BigIntBackend<V> &augend, const BigIntBackend<V> &addend);

        template<class V>
        friend BigIntBackend<V> operator-(const BigIntBackend<V> &minuend, const BigIntBackend<V> &subtrahend);

        template<class V>
        friend BigIntBackend<V> operator*(const BigIntBackend<V> &multiplier, const BigIntBackend<V> &multiplicand);

        template<class V>
        friend BigIntBackend<V> operator/(const BigIntBackend<V> &dividend, const BigIntBackend<V> &divisor);

        template<class V>
        friend BigIntBackend<V> operator%(const BigIntBackend<V> &dividend, const BigIntBackend<V> &divisor);

        BigIntBackend<T> operator<<(const SizeType &shiftBy) const;

        BigIntBackend<T> operator~() const;

        BigIntBackend<T> operator-() const;

        bool operator==(const BigIntBackend<T> &secondOperand) const;

        std::strong_ordering operator<=>(const BigIntBackend<T> &secondOperand) const;

        template<class Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
        explicit operator Value() const {
            const std::size_t outputSize = sizeof(Value);
            const std::size_t requiredPieceCount = outputSize / sizeof(T);

            const BigIntBackend <T> &current = *this;

            if (current.pieces.size() > requiredPieceCount) {
                throw std::logic_error("Cannot cast BigIntBackend to given type - value is too big.");
            }

            Value castedValue = 0b0;

            std::size_t shiftSize = 0;

            for (auto piece: pieces) {
                castedValue |= (((Value) piece) << shiftSize);
                shiftSize += sizeof(T) * 8;
            }

            return castedValue;
        }

        std::string toString() const;

        template<class V>
        friend std::ostream &operator<<(std::ostream &, BigIntBackend<V>);

    private:
        void normalize();

        T getFillValue() const;

        template<class V>
        friend std::pair<BigIntBackend<V>, BigIntBackend<V>>
        longDivision(const BigIntBackend<V> &inDividend, const BigIntBackend<V> &inDivisor);

        static std::pair<const BigIntBackend<T> &, const BigIntBackend<T> &>
        sortBySize(const BigIntBackend<T> &first, const BigIntBackend<T> &second);
    };
}

#endif //BIG_NUMBERS_BIG_INT_HPP
