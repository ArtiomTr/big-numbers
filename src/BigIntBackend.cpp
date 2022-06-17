#include "BigIntBackend.h"

#include <limits>
#include <regex>
#include <bitset>

#include "IsomorphicMath.h"
#include "VectorUtils.h"

namespace BigNumbers {
    template<class T>
    std::pair<const BigIntBackend<T> &, const BigIntBackend<T> &>
    BigIntBackend<T>::sortBySize(const BigIntBackend<T> &first, const BigIntBackend<T> &second) {
        if (first.pieces.size() > second.pieces.size()) {
            return {second, first};
        } else {
            return {first, second};
        }
    }

    template<class T>
    T BigIntBackend<T>::getFillValue() const {
        return sign ? std::numeric_limits<T>::max() : 0b0;
    }

    template<class T>
    std::string BigIntBackend<T>::toString() const {
        std::stringstream output;
        output << (sign ? '-' : '+');

        for (auto it = pieces.rbegin(); it != pieces.rend(); ++it) {
            std::bitset<BigIntBackend<T>::PIECE_SIZE> buff(*it);
            output << buff.to_string();

        }
        output << " (length " << pieces.size() << ")";

        return output.str();
    }

    template<class T>
    BigIntBackend<T>::BigIntBackend() : pieces(std::vector<T>()), sign(0) {

    }

    template<class T>
    BigIntBackend<T> operator+(const BigIntBackend<T> &augend, const BigIntBackend<T> &addend) {
        BigIntBackend<T> sum{};

        const auto &[_, longestSummand] = BigIntBackend<T>::sortBySize(augend, addend);

        auto firstIt = augend.pieces.begin(), secondIt = addend.pieces.begin();

        T carry = 0;

        while (firstIt != augend.pieces.end() || secondIt != addend.pieces.end()) {
            T augendPiece = firstIt != augend.pieces.end() ? *firstIt : augend.getFillValue();
            T addendPiece = secondIt != addend.pieces.end() ? *secondIt : addend.getFillValue();

            T value = augendPiece + addendPiece + carry;

            carry = (augendPiece > value) || (addendPiece > value) ||
                    (carry && (augendPiece == value || addendPiece == value));

            sum.pieces.push_back(value);

            if (firstIt != augend.pieces.end())
                ++firstIt;

            if (secondIt != addend.pieces.end())
                ++secondIt;
        }

        std::bitset<BigIntBackend<T>::PIECE_SIZE> additional(augend.getFillValue() + addend.getFillValue() + carry);

        sum.sign = additional[BigIntBackend<T>::PIECE_SIZE - 1];

        if (additional != sum.getFillValue()) {
            sum.pieces.push_back(additional.to_ulong());
        }

        return sum;
    }

    template<class T>
    BigIntBackend<T> operator-(const BigIntBackend<T> &minuend, const BigIntBackend<T> &subtrahend) {
        return minuend + (-subtrahend);
    }

    template<class T>
    BigIntBackend<T> BigIntBackend<T>::operator~() const {
        BigIntBackend<T> out = *this;

        for (T &piece: out.pieces) {
            piece = ~piece;
        }

        return out;
    }

    template<class T>
    BigIntBackend<T> BigIntBackend<T>::operator-() const {
        if (pieces.empty()) {
            return *this;
        }

        BigIntBackend<T> output = ~(*this) + BigIntBackend<T>((T) 1);
        output.sign = !sign;
        return output;
    }

    template<class T>
    BigIntBackend<T> BigIntBackend<T>::operator<<(const SizeType &shiftBy) const {
        BigIntBackend<T> output;
        output.sign = sign;
        output.pieces.resize(pieces.size());

        SizeType pieceShift = shiftBy % BigIntBackend<T>::PIECE_SIZE;
        SizeType pieceShiftComplement = BigIntBackend<T>::PIECE_SIZE - pieceShift;

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

        for (std::size_t i = 1; i < pieces.size(); ++i) {
            output.pieces[i] = (pieces[i] << pieceShift) |
                               (pieces[i - 1] >> pieceShiftComplement);
        }

        T fillValue = output.getFillValue();

        T additionalPiece = (pieces.back() >> pieceShiftComplement) |
                            (fillValue << pieceShift);

        if (additionalPiece != fillValue) {
            output.pieces.push_back(additionalPiece);
        }

        SizeType emptyPieceCount = shiftBy / BigIntBackend<T>::PIECE_SIZE;
        if (emptyPieceCount > 0) {
            output.pieces.insert(output.pieces.begin(), emptyPieceCount, 0);
        }

        return output;
    }

    template<class T>
    BigIntBackend<T> operator*(const BigIntBackend<T> &multiplier, const BigIntBackend<T> &multiplicand) {
        using SizeType = typename BigIntBackend<T>::SizeType;

        const auto &[shortestMultiplicand, longestMultiplicand] = BigIntBackend<T>::sortBySize(multiplier,
                                                                                               multiplicand);

        BigIntBackend<T> product;
        product.sign = multiplier.sign ^ multiplicand.sign;

        T mask = 0b1;

        std::size_t i = 0;
        for (T currentPiece: shortestMultiplicand.pieces) {
            for (SizeType j = 0; j < BigIntBackend<T>::PIECE_SIZE; ++j) {
                T currentMask = mask << j;

                if (currentPiece & currentMask) {
                    BigIntBackend<T> summand = (longestMultiplicand
                            << (SizeType) (j + i * BigIntBackend<T>::PIECE_SIZE));
                    product = product + summand;
                }
            }
            ++i;
        }

        return product;
    }

    template<class V>
    std::pair<BigIntBackend<V>, BigIntBackend<V>>
    longDivision(const BigIntBackend<V> &inDividend, const BigIntBackend<V> &inDivisor) {
        const BigIntBackend<V> dividend = IsomorphicMath::abs(inDividend);
        const BigIntBackend<V> divisor = IsomorphicMath::abs(inDivisor);

        if (divisor == BigIntBackend<V>(0)) {
            throw std::logic_error("Cannot divide by zero.");
        }

        BigIntBackend<V> remainder;
        remainder.pieces.insert(remainder.pieces.begin(), 0);
        BigIntBackend<V> quotient;

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

            quotient.pieces.insert(quotient.pieces.begin(), static_cast<V>(quotientBits.to_ulong()));
        }

        quotient.normalize();
        uint8_t sign = inDividend.sign ^ inDivisor.sign;

        remainder.normalize();

        return {(sign ? -quotient : quotient), remainder};
    }

    template<class V>
    BigIntBackend<V> operator%(const BigIntBackend<V> &dividend, const BigIntBackend<V> &divisor) {
        auto output = longDivision(dividend, divisor);

        return output.second;
    }

    template<class V>
    BigIntBackend<V> operator/(const BigIntBackend<V> &dividend, const BigIntBackend<V> &divisor) {
        auto output = longDivision(dividend, divisor);

        return output.first;
    }

    template<class T>
    std::strong_ordering BigIntBackend<T>::operator<=>(const BigIntBackend<T> &secondOperand) const {
        const BigIntBackend<T> &firstOperand = *this;

        if (firstOperand.sign != secondOperand.sign) {
            return secondOperand.sign <=> firstOperand.sign;
        }

        if (firstOperand.pieces.size() != secondOperand.pieces.size()) {
            return firstOperand.pieces.size() <=> secondOperand.pieces.size();
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
    bool BigIntBackend<T>::operator==(const BigIntBackend<T> &secondOperand) const {
        return (*this <=> secondOperand) == std::strong_ordering::equal;
    }

    template<class T>
    void BigIntBackend<T>::normalize() {
        trimBack(pieces, getFillValue());
    }

    template<class V>
    std::ostream &operator<<(std::ostream &out, BigIntBackend<V> value) {
        BigIntBackend<V> zero(0);

        if (value == zero) {
            out << '0';

            return out;
        }

        if (value < zero) {
            value = -value;
            out << '-';
        }

        BigIntBackend<V> ten(10);
        std::string valueAsString;

        while (value > zero) {
            char digit = static_cast<char>(value % ten);
            value = value / ten;
            valueAsString += static_cast<char>(digit + '0');
        }

        std::reverse(valueAsString.begin(), valueAsString.end());
        out << valueAsString;

        return out;
    }

    template
    class BigIntBackend<uint8_t>;

    template BigIntBackend<uint8_t>
    operator+(const BigIntBackend<uint8_t> &augend, const BigIntBackend<uint8_t> &addend);

    template BigIntBackend<uint8_t>
    operator-(const BigIntBackend<uint8_t> &minuend, const BigIntBackend<uint8_t> &subtrahend);

    template BigIntBackend<uint8_t>
    operator*(const BigIntBackend<uint8_t> &multiplier, const BigIntBackend<uint8_t> &multiplicand);

    template BigIntBackend<uint8_t>
    operator/(const BigIntBackend<uint8_t> &dividend, const BigIntBackend<uint8_t> &divisor);

    template BigIntBackend<uint8_t>
    operator%(const BigIntBackend<uint8_t> &dividend, const BigIntBackend<uint8_t> &divisor);

    template std::ostream &operator<<(std::ostream &out, BigIntBackend<uint8_t> value);
}