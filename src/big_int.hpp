#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <vector>
#include <string>

#include "padding.hpp"

template<class T = uint8_t, class P = default_pad<T>>
class big_int {
private:
    uint8_t sign;
    std::vector<T> pieces;
    typedef typename std::vector<T>::size_type size_type;
public:

    explicit big_int(std::vector<T> pieces, uint8_t sign);

    big_int<T, P> operator+(const big_int<T, P> &summand) const;

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
