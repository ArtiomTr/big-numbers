#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <vector>
#include <array>
#include <type_traits>
#include <numeric>
#include <string>
#include <iostream>

#include "padding.hpp"

template<class T = uint8_t, class P = default_pad<T>>
class big_int {
private:
    uint8_t sign;
    std::vector<T> pieces;
    typedef typename std::vector<T>::size_type size_type;

    static constexpr std::size_t get_box_count(std::size_t value_size) {
        std::size_t count = value_size / sizeof(T);

        return count + (value_size > count * sizeof(T));
    }

public:
    explicit big_int(std::vector<T> pieces, uint8_t sign);

    template<typename Value, std::enable_if_t<std::is_integral<Value>::value, bool> = true>
    explicit big_int(Value value): sign(value < 0) {
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

    big_int<T, P> operator+(const big_int<T, P> &summand) const;

    big_int<T, P> operator-(const big_int<T, P> &subtrahend) const;

    big_int<T, P> operator*(const big_int<T, P> &multiplicand) const;

    big_int<T, P> operator<<(const size_type &shift_by) const;

    big_int<T, P> operator~() const;

    big_int<T, P> operator-() const;

    std::string binary_str() const;

    static std::size_t get_box_size();

private:
    T get_fill_value() const;

    static big_int<T, P> get_shortest(const big_int<T, P> &first, const big_int<T, P> &second);

    static big_int<T, P> get_longest(const big_int<T, P> &first, const big_int<T, P> &second);
};

template<typename T = uint8_t>
big_int<T> parse_big_int(std::string source);

template<typename T>
std::vector<T> split_into_boxes(const std::vector<uint8_t> &bits);

#endif //BIG_NUMBERS_BIG_INT_HPP
