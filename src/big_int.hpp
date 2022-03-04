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

    big_int<T> operator+(const big_int<T> &summand);

    big_int<T> operator*(const big_int<T> &multiplicand);

    big_int<T> operator<<(const size_type &shift_by) const;

    std::string binary_str();
private:
    static big_int<T> get_shortest(const big_int<T> &first, const big_int<T> &second);

    static big_int<T> get_longest(const big_int<T> &first, const big_int<T> &second);
};

#include "big_int.cpp"

#endif //BIG_NUMBERS_BIG_INT_HPP
