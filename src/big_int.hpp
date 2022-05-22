#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <vector>
#include <array>
#include <type_traits>
#include <numeric>
#include <string>
#include <iostream>

#include "padding.hpp"

template<class T>
class big_int_debugger;

template<class T = uint8_t>
class big_int {
private:
    friend class big_int_debugger<T>;

private:
    uint8_t sign;
    std::vector<T> pieces;
    pad<T> *value_pad;

    using size_type = typename std::vector<T>::size_type;

    static constexpr std::size_t get_box_count(std::size_t value_size) {
        std::size_t count = value_size / sizeof(T);

        return count + (value_size > count * sizeof(T));
    }

public:
    explicit big_int(std::vector<T> pieces, uint8_t sign);

    explicit big_int(std::vector<T> pieces, uint8_t sign, pad<T> *value_pad);

    template<class Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
    explicit big_int(Value value): sign(value < 0), value_pad(new default_pad<T>()) {
        using bytearray = std::array<std::byte, sizeof(Value)>;
        const auto &bytes = std::bit_cast<bytearray, Value>(value);

        pieces.resize(get_box_count(sizeof(Value)));
        std::fill(pieces.begin(), pieces.end(), 0);

        for (std::size_t i = 0; i < bytes.size(); ++i) {
            std::size_t box_index = i / sizeof(T);
            std::size_t offset = (i % sizeof(T)) * std::numeric_limits<uint8_t>::digits;

            pieces[box_index] |= ((T) (bytes[i]) << offset);
        }
    }

    template<class V>
    friend big_int<V> operator+(const big_int<V> &augend, const big_int<V> &addend);

    template<class V>
    friend big_int<V> operator-(const big_int<V> &minuend, const big_int<V> &subtrahend);

    template<class V>
    friend big_int<V> operator*(const big_int<V> &multiplier, const big_int<V> &multiplicand);

    big_int<T> operator<<(const size_type &shift_by) const;

    big_int<T> operator~() const;

    big_int<T> operator-() const;

    bool operator==(const big_int<T> &second_operand) const;

    std::strong_ordering operator<=>(const big_int<T> &second_operand) const;

    std::string binary_str() const;

    static std::size_t get_box_size();

private:
    T get_fill_value() const;

    static std::pair<big_int<T>, big_int<T>> sort_by_size(const big_int<T> &first, const big_int<T> &second);
};

template<class T = uint8_t>
big_int<T> parse_big_int(std::string source);

template<class T>
std::vector<T> split_into_boxes(const std::vector<uint8_t> &bits);

#endif //BIG_NUMBERS_BIG_INT_HPP
