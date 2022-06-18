#include "BigIntBackend.h"

#include <limits>
#include <regex>
#include <bitset>

#include "VectorUtils.h"

namespace BigNumbers {
    template<class T>
    BigIntBackend<T>::BigIntBackend() : pieces(std::vector<T>()), sign(0) {

    }

    template<class T>
    BigIntBackend<T>::BigIntBackend(std::vector<T> pieces, uint8_t sign): pieces(pieces), sign(sign) {

    }

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
    void BigIntBackend<T>::add(const BigIntBackend<T> &addend) {
        auto firstIt = pieces.begin();
        auto secondIt = addend.pieces.begin();

        T carry = 0;

        while (firstIt != pieces.end() || secondIt != addend.pieces.end()) {
            T augendPiece = firstIt != pieces.end() ? *firstIt : getFillValue();
            T addendPiece = secondIt != addend.pieces.end() ? *secondIt : addend.getFillValue();

            T value = augendPiece + addendPiece + carry;

            carry = (augendPiece > value) || (addendPiece > value) ||
                    (carry && (augendPiece == value || addendPiece == value));

            if (firstIt != pieces.end()) {
                *firstIt = value;
                ++firstIt;
            } else {
                pieces.push_back(value);
                firstIt = pieces.end();
            }

            if (secondIt != addend.pieces.end())
                ++secondIt;
        }

        std::bitset<BigIntBackend<T>::PIECE_SIZE> additional(getFillValue() + addend.getFillValue() + carry);

        sign = additional[BigIntBackend<T>::PIECE_SIZE - 1];

        if (additional != getFillValue()) {
            pieces.push_back(additional.to_ulong());
        }
    }

    template<class T>
    void BigIntBackend<T>::subtract(BigIntBackend<T> subtrahend) {
        subtrahend.negate();
        add(subtrahend);
    }

    template<class T>
    void BigIntBackend<T>::invert() {
        for (T &piece: pieces) {
            piece = ~piece;
        }
    }

    template<class T>
    void BigIntBackend<T>::negate() {
        if (pieces.empty()) {
            return;
        }

        invert();
        add(BigIntBackend<T>((T) 1));
        sign = !sign;
    }

    template<class T>
    void BigIntBackend<T>::shiftLeft(const SizeType &shiftBy) {
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

        T previous = pieces.front();
        if (!pieces.empty()) {
            pieces.front() <<= pieceShift;
        }

        for (std::size_t i = 1; i < pieces.size(); ++i) {
            T current = pieces[i];
            pieces[i] <<= pieceShift;
            pieces[i] |= (previous >> pieceShiftComplement);
            previous = current;
        }

        T fillValue = getFillValue();

        T additionalPiece = (previous >> pieceShiftComplement) |
                            (fillValue << pieceShift);

        if (additionalPiece != fillValue) {
            pieces.push_back(additionalPiece);
        }

        SizeType emptyPieceCount = shiftBy / BigIntBackend<T>::PIECE_SIZE;
        if (emptyPieceCount > 0) {
            pieces.insert(pieces.begin(), emptyPieceCount, 0);
        }
    }

    template<class T>
    void BigIntBackend<T>::multiply(const BigIntBackend<T> &multiplicand) {
        const auto &[shortestMultiplicand, longestMultiplicand] = BigIntBackend<T>::sortBySize(*this,
                                                                                               multiplicand);

        BigIntBackend<T> product;
        product.sign = sign ^ multiplicand.sign;

        T mask = 0b1;

        std::size_t i = 0;
        SizeType prevShift = 0;

        BigIntBackend<T> shiftMultiplicand = longestMultiplicand;
        for (T currentPiece: shortestMultiplicand.pieces) {
            for (SizeType j = 0; j < BigIntBackend<T>::PIECE_SIZE; ++j) {
                T currentMask = mask << j;

                if (currentPiece & currentMask) {
                    SizeType currShift = j + i * BigIntBackend<T>::PIECE_SIZE;
                    shiftMultiplicand.shiftLeft(currShift - prevShift);
                    prevShift = currShift;
                    product.add(shiftMultiplicand);
                }
            }
            ++i;
        }

        *this = product;
    }

    template<class T>
    BigIntBackend<T> BigIntBackend<T>::divide(BigIntBackend<T> divisor) {
        if (divisor.compare(BigIntBackend<T>(0)) == 0) {
            throw std::logic_error("Cannot divide by zero.");
        }

        uint8_t outputSign = sign ^ divisor.sign;

        if (sign) {
            negate();
        }

        if (divisor.sign) {
            divisor.negate();
        }

        BigIntBackend<T> remainder;
        remainder.pieces.insert(remainder.pieces.begin(), 0);

        for (auto piece = pieces.rbegin(); piece != pieces.rend(); ++piece) {
            std::bitset<BigIntBackend<T>::PIECE_SIZE> pieceBits(*piece);
            std::bitset<BigIntBackend<T>::PIECE_SIZE> quotientBits(0b0);

            for (std::size_t j = BigIntBackend<T>::PIECE_SIZE; j > 0; --j) {
                std::size_t bitIndex = j - 1;
                remainder.shiftLeft(1);

                std::bitset<BigIntBackend<T>::PIECE_SIZE> remainderBits(remainder.pieces.front());
                remainderBits.set(0, pieceBits[bitIndex]);
                remainder.pieces.front() = static_cast<T>(remainderBits.to_ulong());

                if (remainder.compare(divisor) >= 0) {
                    remainder.subtract(divisor);
                    quotientBits.set(bitIndex, true);
                }
            }
            *piece = static_cast<T>(quotientBits.to_ulong());
        }

        normalize();
        if (outputSign) {
            negate();
        }
        remainder.normalize();

        return remainder;
    }

    template<class T>
    int8_t BigIntBackend<T>::compare(const BigIntBackend<T> &secondOperand) const {
        const BigIntBackend<T> &firstOperand = *this;

        if (firstOperand.sign != secondOperand.sign) {
            if (firstOperand.sign > secondOperand.sign) {
                return -1;
            }

            return 1;
        }

        if (firstOperand.pieces.size() != secondOperand.pieces.size()) {
            if (firstOperand.pieces.size() > secondOperand.pieces.size()) {
                return 1;
            }

            return -1;
        }

        auto firstIt = firstOperand.pieces.rbegin(), secondIt = secondOperand.pieces.rbegin();

        while (firstIt != firstOperand.pieces.rend() && secondIt != secondOperand.pieces.rend()) {
            if (*firstIt > *secondIt) {
                return 1;
            } else if (*firstIt < *secondIt) {
                return -1;
            }

            ++firstIt;
            ++secondIt;
        }

        return 0;
    }

    template<class T>
    void BigIntBackend<T>::normalize() {
        trimBack(pieces, getFillValue());
    }

    template<class V>
    std::ostream &operator<<(std::ostream &out, BigIntBackend<V> value) {
        BigIntBackend<V> zero(0);

        if (value.compare(zero) == 0) {
            out << '0';

            return out;
        }

        if (value.compare(zero) < 0) {
            value.negate();
            out << '-';
        }

        BigIntBackend<V> ten(10);
        std::string valueAsString;

        while (value.compare(zero) > 0) {
            valueAsString += static_cast<char>(value.divide(ten)) + '0';
        }

        std::reverse(valueAsString.begin(), valueAsString.end());
        out << valueAsString;

        return out;
    }

    template<class T>
    std::vector<T> &BigIntBackend<T>::accessPieces() {
        return pieces;
    }

    template<class T>
    std::vector<T> BigIntBackend<T>::accessPieces() const {
        return pieces;
    }

    template<class T>
    int32_t BigIntBackend<T>::getSign() const {
        return sign;
    }

    template
    class BigIntBackend<uint8_t>;

    template std::ostream &operator<<(std::ostream &out, BigIntBackend<uint8_t> value);
}