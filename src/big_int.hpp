#ifndef BIG_NUMBERS_BIG_INT_HPP
#define BIG_NUMBERS_BIG_INT_HPP

#include <vector>
#include <string>

template<typename T>
class big_int {
private:
    uint8_t sign;
    std::vector<T> pieces;
    typedef typename std::vector<T>::size_type size_type;
public:

    explicit big_int(std::vector<T> pieces, uint8_t sign);

    big_int<T> operator+(const big_int<T> &summand) const;

    big_int<T> operator*(const big_int<T> &multiplicand) const;

    big_int<T> operator<<(const size_type &shift_by) const;

    big_int<T> operator~() const;

    big_int<T> operator-() const;

    std::string binary_str() const;

    static std::size_t get_box_size();

private:
    T get_fill_value() const;

    static big_int<T> get_shortest(const big_int<T> &first, const big_int<T> &second);

    static big_int<T> get_longest(const big_int<T> &first, const big_int<T> &second);
};

template<typename T>
big_int<T> parse_big_int(std::string source);

#include "big_int.cpp"

#endif //BIG_NUMBERS_BIG_INT_HPP
