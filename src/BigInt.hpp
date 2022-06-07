#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <vector>
#include <array>
#include <type_traits>
#include <numeric>
#include <string>
#include <stdexcept>

template<class T>
class BigIntDebugger;

template<class T = uint8_t>
class BigInt {
private:
    friend class BigIntDebugger<T>;
private:
    uint8_t sign;
    std::vector<T> pieces;

    using SizeType = typename std::vector<T>::size_type;
public:
    explicit BigInt(std::vector<T> initialPieces, uint8_t initialSign);

    template<class Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
    BigInt(Value value): sign(value < 0) {
        using bytearray = std::array<std::byte, sizeof(Value)>;
        const auto &bytes = std::bit_cast<bytearray, Value>(value);

        pieces.resize(getBoxCount(sizeof(Value)));
        std::fill(pieces.begin(), pieces.end(), 0);

        for (std::size_t i = 0; i < bytes.size(); ++i) {
            std::size_t boxIndex = i / sizeof(T);
            std::size_t offset = (i % sizeof(T)) * std::numeric_limits<uint8_t>::digits;

            pieces[boxIndex] |= ((T) (bytes[i]) << offset);
        }
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

        BigInt<T> current = trim();

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

    BigInt<T> trim() const;

    void pushRight(std::size_t count);

    void padRight(std::size_t width);

    void trimRight();

    std::size_t getWidth() const;

private:
    static constexpr std::size_t getBoxCount(std::size_t valueSize) {
        std::size_t count = valueSize / sizeof(T);

        return count + (valueSize > count * sizeof(T));
    }

    static constexpr std::size_t boxSize = sizeof(T) * 8;

    T getFillValue() const;

    template<class V>
    friend std::pair<BigInt<V>, BigInt<V>> longDivision(const BigInt<V> &inDividend, const BigInt<V> &inDivisor);

    static std::pair<BigInt<T>, BigInt<T>> sortBySize(const BigInt<T> &first, const BigInt<T> &second);
};

template<class T = uint8_t>
BigInt<T> parseBigInt(std::string source);

#endif //BIG_NUMBERS_BIG_INT_HPP
