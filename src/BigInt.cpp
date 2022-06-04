#include "BigInt.hpp"

#include <limits>
#include <regex>
#include <bitset>

#include "IsomorphicMath.hpp"
#include "ParsingUtils.h"

template<class T>
std::pair<BigInt<T>, BigInt<T>> BigInt<T>::sortBySize(const BigInt<T> &first, const BigInt<T> &second) {
    if (first.pieces.size() > second.pieces.size()) {
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
std::size_t BigInt<T>::getBoxSize() {
    return sizeof(T) * std::numeric_limits<uint8_t>::digits;
}

template<class T>
std::string BigInt<T>::toString() const {
    std::string output = sign ? "-" : "+";

    T mask = 0b1;

    // Careful! This loop intentionally iterates from pieces.size() till 1, because loop from pieces.size() - 1 till 0
    //     will cause unsigned overflow, what will lead to an infinite loop.
    for (SizeType i = pieces.size(); i > 0; --i) {
        T piece = pieces[i - 1];
        std::string numberStr;
        for (SizeType j = 0; j < BigInt<T>::getBoxSize(); ++j) {
            numberStr += ('0' + (piece & mask));
            piece >>= 1;
        }
        output += std::string(numberStr.rbegin(), numberStr.rend());
    }

    return output;
}

template<class T>
BigInt<T>::BigInt(std::vector<T> initialPieces, uint8_t initialSign, Pad<T> *pad):
        pieces(std::move(initialPieces)),
        sign(initialSign),
        valuePad(pad) {
    if (initialSign != 0 && initialSign != 1) {
        throw std::invalid_argument("Sign should be 0 for positive numbers or 1 for negative");
    }
}

template<class T>
BigInt<T>::BigInt(std::vector<T> initialPieces, uint8_t initialSign):
        BigInt(initialPieces, initialSign, new DefaultPad<T>()) {
}

template<class T>
BigInt<T> operator+(const BigInt<T> &augend, const BigInt<T> &addend) {
    using SizeType = typename BigInt<T>::SizeType;

    BigInt<T> sum({}, 0);

    const auto &[_, longestSummand] = BigInt<T>::sortBySize(augend, addend);

    SizeType maxSize = longestSummand.pieces.size();
    sum.pieces.resize(maxSize);

    PaddedList<T> *firstSummandPieces = augend.valuePad->padForSum(augend.pieces, augend.sign);
    PaddedList<T> *secondSummandPieces = addend.valuePad->padForSum(addend.pieces, addend.sign);

    T carry = 0;

    for (SizeType i = 0; i < maxSize; ++i) {
        T value = firstSummandPieces->get(i) + secondSummandPieces->get(i) + carry;

        carry = (firstSummandPieces->get(i) > value) || (secondSummandPieces->get(i) > value) ||
                (carry && (firstSummandPieces->get(i) == value || secondSummandPieces->get(i) == value));

        sum.pieces[i] = value;
    }

    T additional = firstSummandPieces->get(maxSize) + secondSummandPieces->get(maxSize) + carry;

    sum.sign = additional >> (BigInt<T>::getBoxSize() - 1);

    if (additional != sum.getFillValue()) {
        sum.pieces.push_back(additional);
    }

    delete firstSummandPieces;
    delete secondSummandPieces;

    return sum;
}

template<class T>
BigInt<T> operator-(const BigInt<T> &minuend, const BigInt<T> &subtrahend) {
    return minuend + (-subtrahend);
}

template<class T>
BigInt<T> BigInt<T>::operator~() const {
    BigInt<T> out({}, !sign);
    out.pieces.resize(pieces.size());

    for (SizeType i = 0; i < pieces.size(); ++i) {
        out.pieces[i] = ~pieces[i];
    }

    return out;
}

template<class T>
BigInt<T> BigInt<T>::operator-() const {
    BigInt<T> output = ~(*this) + (BigInt<T>) (T) 1;
    output.sign = !sign;
    return output;
}

template<class T>
BigInt<T> BigInt<T>::operator<<(const SizeType &shiftBy) const {
    BigInt<T> output({}, sign);

    output.pieces.resize(pieces.size());

    SizeType pieceShift = shiftBy % BigInt<T>::getBoxSize();
    SizeType pieceShiftComplement = BigInt<T>::getBoxSize() - pieceShift;

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
        output.pieces[0] = (pieces[0] << pieceShift);
    }

    for (SizeType i = 1; i < pieces.size(); ++i) {
        output.pieces[i] = (pieces[i] << pieceShift) |
                           (pieces[i - 1] >> pieceShiftComplement);
    }

    T fill_value = output.getFillValue();

    T additionalPiece = (pieces[output.pieces.size() - 1] >> pieceShiftComplement) |
                        (fill_value << pieceShift);

    if (additionalPiece != fill_value) {
        output.pieces.push_back(additionalPiece);
    }

    SizeType empty_piece_count = shiftBy / BigInt<T>::getBoxSize();
    if (empty_piece_count > 0) {
        output.pieces.insert(output.pieces.begin(), empty_piece_count, 0);
    }

    return output;
}

template<class T>
BigInt<T> operator*(const BigInt<T> &multiplier, const BigInt<T> &multiplicand) {
    using SizeType = typename BigInt<T>::SizeType;

    const auto &[shortestMultiplicand, longestMultiplicand] = BigInt<T>::sortBySize(multiplier, multiplicand);

    SizeType minSize = shortestMultiplicand.pieces.size();
    SizeType shiftSize = BigInt<T>::getBoxSize();

    BigInt<T> product({}, multiplier.sign ^ multiplicand.sign);

    T mask = 0b1;

    for (SizeType i = 0; i < minSize; ++i) {
        for (SizeType j = 0; j < shiftSize; ++j) {
            T currentMask = mask << j;
            T currentPiece = shortestMultiplicand.pieces[i];

            if (currentPiece & currentMask) {
                BigInt<T> summand = (longestMultiplicand << (SizeType) (j + i * shiftSize));
                product = product + summand;
            }
        }
    }

    return product;
}

template<class V>
std::pair<BigInt<V>, BigInt<V>> longDivision(const BigInt<V> &inDividend, const BigInt<V> &inDivisor) {
    BigInt<V> dividend = IsomorphicMath::abs(inDividend).trim();
    BigInt<V> divisor = IsomorphicMath::abs(inDivisor).trim();

    if (divisor == BigInt<V>(0)) {
        throw std::logic_error("Cannot divide by zero.");
    }

    BigInt<V> remainder(0);
    std::vector<V> quotientPieces;

    constexpr std::size_t BIT_COUNT = sizeof(V) * 8;

    for (auto piece = dividend.pieces.rbegin(); piece != dividend.pieces.rend(); ++piece) {
        std::bitset<BIT_COUNT> pieceBits(*piece);
        std::bitset<BIT_COUNT> quotientBits(0b0);

        for (std::size_t j = BIT_COUNT; j > 0; --j) {
            std::size_t bitIndex = j - 1;
            remainder = remainder << 1;

            std::bitset<BIT_COUNT> remainderBits(remainder.pieces[0]);
            remainderBits.set(0, pieceBits[bitIndex]);
            remainder.pieces[0] = static_cast<V>(remainderBits.to_ulong());

            if (remainder >= divisor) {
                remainder = remainder - divisor;
                quotientBits.set(bitIndex, true);
            }
        }

        quotientPieces.push_back(static_cast<V>(quotientBits.to_ulong()));
    }

    std::reverse(quotientPieces.begin(), quotientPieces.end());

    BigInt<V> quotient(quotientPieces, 0);
    uint8_t sign = inDividend.sign ^ inDivisor.sign;

    return {(sign ? -quotient : quotient).trim(), remainder.trim()};
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
BigInt<T> BigInt<T>::trim() const {
    SizeType end;
    T fillValue = getFillValue();
    for (end = pieces.size(); end > 0; --end) {
        if (pieces[end - 1] != fillValue) {
            break;
        }
    }

    if (end == pieces.size()) {
        return *this;
    }

    return BigInt<T>(std::vector<T>(pieces.begin(), pieces.begin() + end), sign);
}

template<class T>
std::strong_ordering BigInt<T>::operator<=>(const BigInt<T> &secondOperand) const {
    const BigInt<T> &firstOperand = *this;

    if (firstOperand.sign != secondOperand.sign) {
        return secondOperand.sign <=> firstOperand.sign;
    }

    PaddedList<T> *firstOperandPieces = valuePad->padForSum(firstOperand.pieces, firstOperand.sign);
    PaddedList<T> *secondOperandPieces = valuePad->padForSum(secondOperand.pieces, secondOperand.sign);

    SizeType longest = std::max(firstOperand.pieces.size(), secondOperand.pieces.size());

    for (SizeType i = longest; i > 0; --i) {
        auto result = firstOperandPieces->get(i - 1) <=> secondOperandPieces->get(i - 1);

        if (result != std::strong_ordering::equal) {
            return result;
        }
    }

    return std::strong_ordering::equal;
}

template<class T>
bool BigInt<T>::operator==(const BigInt<T> &secondOperand) const {
    return (*this <=> secondOperand) == std::strong_ordering::equal;
}

template<class T>
BigInt<T> parseBigInt(std::string source) {
    std::regex bigIntRegex("^-?\\d+$");

    if (!std::regex_match(source, bigIntRegex)) {
        throw std::invalid_argument("Invalid BigInt format");
    }

    uint8_t sign = source[0] == '-';

    if (sign) {
        source.erase(source.begin());
    }

    std::vector<T> pieces = integralSourceToBinary<T>(source);

    std::reverse(pieces.begin(), pieces.end());

    BigInt<T> out(pieces, 0);

    return sign ? -out : out;
}

template
class BigInt<uint8_t>;

template BigInt<uint8_t> parseBigInt(std::string source);

template BigInt<uint8_t> operator+(const BigInt<uint8_t> &augend, const BigInt<uint8_t> &addend);

template BigInt<uint8_t> operator-(const BigInt<uint8_t> &minuend, const BigInt<uint8_t> &subtrahend);

template BigInt<uint8_t> operator*(const BigInt<uint8_t> &multiplier, const BigInt<uint8_t> &multiplicand);

template BigInt<uint8_t> operator/(const BigInt<uint8_t> &dividend, const BigInt<uint8_t> &divisor);

template BigInt<uint8_t> operator%(const BigInt<uint8_t> &dividend, const BigInt<uint8_t> &divisor);
