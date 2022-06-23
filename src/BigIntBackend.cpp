#include "BigIntBackend.h"

#include <limits>
#include <regex>
#include <bitset>

#include "VectorUtils.h"
#include "config.h"

namespace BigNumbers {
    template<class T>
    BigIntBackend<T>::BigIntBackend() : pieces(std::vector<T>()), isNegative(false) {

    }

    template<class T>
    BigIntBackend<T>::BigIntBackend(bool sign, std::vector<T> pieces): pieces(pieces), isNegative(sign) {

    }

    template<class T>
    BigIntBackend<T>::BigIntBackend(unsigned char *bytes, std::size_t count): isNegative(false) {
        std::bitset<PIECE_SIZE> buffer;
        std::size_t bufferIndex = 0;

        std::size_t pieceCount = count / sizeof(T);
        pieceCount += (count > pieceCount * sizeof(T));
        pieces.resize(pieceCount);

        constexpr const std::size_t BITS = std::numeric_limits<unsigned char>::digits;

        std::size_t pieceIndex = 0;
        for (std::size_t byteIndex = 0; byteIndex < count; ++byteIndex) {
            std::bitset<8> input(bytes[byteIndex]);

            if (byteIndex == count - 1) {
                isNegative = input[BITS - 1];
            }

            for (std::size_t i = 0; i < BITS; ++i) {
                buffer[bufferIndex] = input[i];
                ++bufferIndex;

                if (bufferIndex == PIECE_SIZE) {
                    pieces[pieceIndex] = buffer.to_ulong();
                    ++pieceIndex;
                    buffer.reset();
                    bufferIndex = 0;
                }
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
        return isNegative ? std::numeric_limits<T>::max() : 0b0;
    }

    template<class T>
    std::string BigIntBackend<T>::toBinaryString() const {
        std::stringstream output;

        if (isNegative) {
            output << '-';
        }

        for (auto it = pieces.rbegin(); it != pieces.rend(); ++it) {
            std::bitset<BigIntBackend<T>::PIECE_SIZE> buff(*it);
            output << buff.to_string();

        }
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

        isNegative = additional[BigIntBackend<T>::PIECE_SIZE - 1];

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
        invert();
        isNegative = !isNegative;
        add(BigIntBackend<T>((T) 1));
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
        auto sortedValues = BigIntBackend<T>::sortBySize(*this, multiplicand);

        const auto &shortestMultiplicand = sortedValues.first;
        const auto &longestMultiplicand = sortedValues.second;

        BigIntBackend<T> product;
        product.isNegative = isNegative ^ multiplicand.isNegative;

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

        uint8_t outputSign = isNegative ^ divisor.isNegative;

        if (isNegative) {
            negate();
        }

        if (divisor.isNegative) {
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
    int8_t BigIntBackend<T>::compare(BigIntBackend<T> secondOperand) const {
        BigIntBackend<T> firstOperand = *this;
        secondOperand.normalize();
        firstOperand.normalize();

        if (firstOperand.isNegative != secondOperand.isNegative) {
            if (firstOperand.isNegative > secondOperand.isNegative) {
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

    template<class T>
    std::string BigIntBackend<T>::toString() const {
        BigIntBackend<T> zero(0);

        if (compare(zero) == 0) {
            return "0";
        }

        std::string valueAsString;

        auto value = *this;
        if (value.isNegative) {
            value.negate();
        }

        BigIntBackend<T> ten(10);

        while (value.compare(zero) > 0) {
            valueAsString += static_cast<char>(value.divide(ten)) + '0';
        }

        std::reverse(valueAsString.begin(), valueAsString.end());

        if (isNegative) {
            valueAsString = "-" + valueAsString;
        }

        return valueAsString;
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
        return isNegative;
    }

    template<class T>
    std::pair<unsigned char *, std::size_t> BigIntBackend<T>::getBytes() const {
        std::vector<unsigned char> bytes;

        for (auto piece: pieces) {
            auto *pieceBytes = reinterpret_cast<unsigned char *>(&piece);

            for (std::size_t i = sizeof(piece); i > 0; --i) {
                bytes.push_back(pieceBytes[i - 1]);
            }
        }

        auto *convertedBytes = new unsigned char[bytes.size()];
        std::copy(bytes.begin(), bytes.end(), convertedBytes);

        return {convertedBytes, bytes.size()};
    }

    // Required for debugging
    template
    class BigIntBackend<uint8_t>;

    // Final result
    template
    class BigIntBackend<PieceType>;

    // TODO: remove this
    // Additional tests
    template
    class BigIntBackend<uint64_t>;
}