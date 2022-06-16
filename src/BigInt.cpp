#include "BigInt.hpp"

#include <limits>
#include <regex>
#include <bitset>

#include "IsomorphicMath.hpp"

namespace BigNumbers {
    template<class T>
    std::pair<const BigInt<T> &, const BigInt<T> &>
    BigInt<T>::sortBySize(const BigInt<T> &first, const BigInt<T> &second) {
        if (first.pieces.getSize() > second.pieces.getSize()) {
            return {second, first};
        } else {
            return {first, second};
        }
    }

    template<class T>
    T BigInt<T>::getFillValue() const {
        return sign ? std::numeric_limits<T>::max() : 0b0;
    }

    template<class T>
    std::string BigInt<T>::toString() const {
        std::stringstream output;
        output << (sign ? '-' : '+');

        for (auto it = pieces.rbegin(); it != pieces.rend(); ++it) {
            std::bitset<BigInt<T>::PIECE_SIZE> buff(*it);
            output << buff.to_string();

        }
        output << " (length " << pieces.getSize() << ")";

        return output.str();
    }

    template<class T>
    BigInt<T>::BigInt() : pieces(DoubleEndedPolynomial<T>()), sign(0) {

    }

    template<class T>
    BigInt<T> operator+(const BigInt<T> &augend, const BigInt<T> &addend) {
        BigInt<T> sum{};

        const auto &[_, longestSummand] = BigInt<T>::sortBySize(augend, addend);

        auto firstIt = augend.pieces.begin(), secondIt = addend.pieces.begin();

        T carry = 0;

        while (firstIt != augend.pieces.end() || secondIt != addend.pieces.end()) {
            T augendPiece = firstIt != augend.pieces.end() ? *firstIt : augend.getFillValue();
            T addendPiece = secondIt != addend.pieces.end() ? *secondIt : addend.getFillValue();

            T value = augendPiece + addendPiece + carry;

            carry = (augendPiece > value) || (addendPiece > value) ||
                    (carry && (augendPiece == value || addendPiece == value));

            sum.pieces.pushBack(value);

            if (firstIt != augend.pieces.end())
                ++firstIt;

            if (secondIt != addend.pieces.end())
                ++secondIt;
        }

        std::bitset<BigInt<T>::PIECE_SIZE> additional(augend.getFillValue() + addend.getFillValue() + carry);

        sum.sign = additional[BigInt<T>::PIECE_SIZE - 1];

        if (additional != sum.getFillValue()) {
            sum.pieces.pushBack(additional.to_ulong());
        }

        return sum;
    }

    template<class T>
    BigInt<T> operator-(const BigInt<T> &minuend, const BigInt<T> &subtrahend) {
        return minuend + (-subtrahend);
    }

    template<class T>
    BigInt<T> BigInt<T>::operator~() const {
        BigInt<T> out = *this;

        for (T &piece: out.pieces) {
            piece = ~piece;
        }

        return out;
    }

    template<class T>
    BigInt<T> BigInt<T>::operator-() const {
        if (pieces.empty()) {
            return *this;
        }

        BigInt<T> output = ~(*this) + BigInt<T>((T) 1);
        output.sign = !sign;
        return output;
    }

    template<class T>
    BigInt<T> BigInt<T>::operator<<(const SizeType &shiftBy) const {
        BigInt<T> output;
        output.sign = sign;
        output.pieces.resize(pieces.getSize());

        SizeType pieceShift = shiftBy % BigInt<T>::PIECE_SIZE;
        SizeType pieceShiftComplement = BigInt<T>::PIECE_SIZE - pieceShift;

        T maskBuilder = 0b0;

        for (SizeType i = 0; i < pieceShiftComplement; ++i) {
            maskBuilder <<= 1;
            maskBuilder += 1;
        }

        if (maskBuilder == 0) {
            maskBuilder = std::numeric_limits<T>::max();
        }

        T mask = ~maskBuilder;

        if (!pieces.empty()) {
            output.pieces.front() = pieces.front() << pieceShift;
        }

        typename DoubleEndedPolynomial<T>::ConstIterator sourceIt;
        typename DoubleEndedPolynomial<T>::Iterator outIt;
        for (sourceIt = ++pieces.begin(), outIt = ++output.pieces.begin();
             sourceIt != pieces.end() && outIt != output.pieces.end();
             ++sourceIt, ++outIt) {
            auto previous = std::prev(sourceIt);
            T newPiece = (*sourceIt << pieceShift) | (*previous >> pieceShiftComplement);

            *outIt = newPiece;
        }

        T fillValue = output.getFillValue();

        T additionalPiece = (pieces.back() >> pieceShiftComplement) |
                            (fillValue << pieceShift);

        if (additionalPiece != fillValue) {
            output.pieces.pushBack(additionalPiece);
        }

        SizeType emptyPieceCount = shiftBy / BigInt<T>::PIECE_SIZE;
        if (emptyPieceCount > 0) {
            extendFront(output.pieces, emptyPieceCount, 0);
        }

        return output;
    }

    template<class T>
    BigInt<T> operator*(const BigInt<T> &multiplier, const BigInt<T> &multiplicand) {
        using SizeType = typename BigInt<T>::SizeType;

        const auto &[shortestMultiplicand, longestMultiplicand] = BigInt<T>::sortBySize(multiplier, multiplicand);

        BigInt<T> product;
        product.sign = multiplier.sign ^ multiplicand.sign;

        T mask = 0b1;

        std::size_t i = 0;
        for (T currentPiece: shortestMultiplicand.pieces) {
            for (SizeType j = 0; j < BigInt<T>::PIECE_SIZE; ++j) {
                T currentMask = mask << j;

                if (currentPiece & currentMask) {
                    BigInt<T> summand = (longestMultiplicand << (SizeType) (j + i * BigInt<T>::PIECE_SIZE));
                    product = product + summand;
                }
            }
            ++i;
        }

        return product;
    }

    template<class V>
    std::pair<BigInt<V>, BigInt<V>> longDivision(const BigInt<V> &inDividend, const BigInt<V> &inDivisor) {
        const BigInt<V> dividend = IsomorphicMath::abs(inDividend);
        const BigInt<V> divisor = IsomorphicMath::abs(inDivisor);

        if (divisor == BigInt<V>(0)) {
            throw std::logic_error("Cannot divide by zero.");
        }

        BigInt<V> remainder;
        remainder.pieces.pushFront(0);
        BigInt<V> quotient;

        constexpr std::size_t BIT_COUNT = sizeof(V) * 8;

        for (auto piece = dividend.pieces.rbegin(); piece != dividend.pieces.rend(); ++piece) {
            std::bitset<BIT_COUNT> pieceBits(*piece);
            std::bitset<BIT_COUNT> quotientBits(0b0);

            for (std::size_t j = BIT_COUNT; j > 0; --j) {
                std::size_t bitIndex = j - 1;
                remainder = remainder << 1;

                std::bitset<BIT_COUNT> remainderBits(remainder.pieces.front());
                remainderBits.set(0, pieceBits[bitIndex]);
                remainder.pieces.front() = static_cast<V>(remainderBits.to_ulong());

                if (remainder >= divisor) {
                    remainder = remainder - divisor;
                    quotientBits.set(bitIndex, true);
                }
            }

            quotient.pieces.pushFront(static_cast<V>(quotientBits.to_ulong()));
        }

        quotient.normalize();
        uint8_t sign = inDividend.sign ^ inDivisor.sign;

        remainder.normalize();

        return {(sign ? -quotient : quotient), remainder};
    }

    template<class V>
    BigInt<V> operator%(const BigInt<V> &dividend, const BigInt<V> &divisor) {
        auto output = longDivision(dividend, divisor);

        return output.second;
    }

    template<class V>
    BigInt<V> operator/(const BigInt<V> &dividend, const BigInt<V> &divisor) {
        auto output = longDivision(dividend, divisor);

        return output.first;
    }

    template<class T>
    std::strong_ordering BigInt<T>::operator<=>(const BigInt<T> &secondOperand) const {
        const BigInt<T> &firstOperand = *this;

        if (firstOperand.sign != secondOperand.sign) {
            return secondOperand.sign <=> firstOperand.sign;
        }

        if (firstOperand.pieces.getSize() != secondOperand.pieces.getSize()) {
            return firstOperand.pieces.getSize() <=> secondOperand.pieces.getSize();
        }

        auto firstIt = firstOperand.pieces.rbegin(), secondIt = secondOperand.pieces.rbegin();

        while (firstIt != firstOperand.pieces.rend() && secondIt != secondOperand.pieces.rend()) {
            auto result = (*firstIt) <=> (*secondIt);

            if (result != std::strong_ordering::equal) {
                return result;
            }

            ++firstIt;
            ++secondIt;
        }

        return std::strong_ordering::equal;
    }

    template<class T>
    bool BigInt<T>::operator==(const BigInt<T> &secondOperand) const {
        return (*this <=> secondOperand) == std::strong_ordering::equal;
    }

    template<class T>
    void BigInt<T>::normalize() {
        trimBack(pieces, getFillValue());
    }

    template
    class BigInt<uint8_t>;

    template BigInt<uint8_t> operator+(const BigInt<uint8_t> &augend, const BigInt<uint8_t> &addend);

    template BigInt<uint8_t> operator-(const BigInt<uint8_t> &minuend, const BigInt<uint8_t> &subtrahend);

    template BigInt<uint8_t> operator*(const BigInt<uint8_t> &multiplier, const BigInt<uint8_t> &multiplicand);

    template BigInt<uint8_t> operator/(const BigInt<uint8_t> &dividend, const BigInt<uint8_t> &divisor);

    template BigInt<uint8_t> operator%(const BigInt<uint8_t> &dividend, const BigInt<uint8_t> &divisor);
}