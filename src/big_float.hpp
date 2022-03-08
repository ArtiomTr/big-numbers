#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "big_int.hpp"

template<typename T>
class big_float {
private:
    big_int<T> exponent;
    big_int<T> mantissa;
public:
    explicit big_float(big_int<T> exponent, big_int<T> mantissa);

    std::string binary_str();
};

template<typename T>
big_float<T> parse_big_float(std::string source, std::size_t mantissa_width);

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
