#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <string>
#include <stdexcept>
#include <bitset>

#include "DoubleEndedPolynomial.h"

namespace BigNumbers {
    template<class T>
    class BigIntDebugger;

    template<class T>
    class BigFloat;

    template<class T = uint8_t>
    class BigInt {
        // Wait, is that spaghetti code? Nah, it's just very friendly class :p
        friend class BigIntDebugger<T>;

        friend class BigFloat<T>;

        template<class V>
        friend BigInt<V> parseBigInt(std::string);

        template<class V>
        friend BigFloat<V> parseBigFloat(std::string, std::size_t);

    private:
        uint8_t sign;
        DoubleEndedPolynomial<T> pieces;

        static constexpr std::size_t PIECE_SIZE = sizeof(T) * 8;
    public:
        using SizeType = typename DoubleEndedPolynomial<T>::SizeType;

        explicit BigInt();

        template<class Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
        BigInt(Value value): sign(value < 0) {
            constexpr std::size_t BIT_COUNT = sizeof(Value) * 8;
            std::bitset<BIT_COUNT> input(value);
            std::bitset<PIECE_SIZE> buffer;
            std::size_t bufferIndex = 0;

            for (std::size_t i = 0; i < BIT_COUNT; ++i) {
                buffer[bufferIndex] = input[i];
                ++bufferIndex;

                if (bufferIndex == PIECE_SIZE) {
                    pieces.pushBack(buffer.to_ulong());
                    buffer.reset();
                    bufferIndex = 0;
                }
            }

            if (buffer.any()) {
                pieces.pushBack(buffer.to_ulong());
            }

            normalize();
        }

        template<class V>
        friend BigInt<V> operator+(const BigInt<V> &augend, const BigInt<V> &addend);

        template<class V>
        friend BigInt<V> operator-(const BigInt<V> &minuend, const BigInt<V> &subtrahend);

        template<class V>
        friend BigInt<V> operator*(const BigInt<V> &multiplier, const BigInt<V> &multiplicand);

        template<class V>
        friend BigInt<V> operator/(const BigInt<V> &dividend, const BigInt<V> &divisor);

        template<class V>
        friend BigInt<V> operator%(const BigInt<V> &dividend, const BigInt<V> &divisor);

        BigInt<T> operator<<(const SizeType &shiftBy) const;

        BigInt<T> operator~() const;

        BigInt<T> operator-() const;

        bool operator==(const BigInt<T> &secondOperand) const;

        std::strong_ordering operator<=>(const BigInt<T> &secondOperand) const;

        template<class Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
        explicit operator Value() const {
            const std::size_t outputSize = sizeof(Value);
            const std::size_t requiredPieceCount = outputSize / sizeof(T);

            const BigInt<T> &current = *this;

            if (current.pieces.size() > requiredPieceCount) {
                throw std::logic_error("Cannot cast BigInt to given type - value is too big.");
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

    private:
        void normalize();

        T getFillValue() const;

        template<class V>
        friend std::pair<BigInt<V>, BigInt<V>> longDivision(const BigInt<V> &inDividend, const BigInt<V> &inDivisor);

        static std::pair<BigInt<T>, BigInt<T>> sortBySize(const BigInt<T> &first, const BigInt<T> &second);
    };
}

#endif //BIG_NUMBERS_BIG_INT_HPP
