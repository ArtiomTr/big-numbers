#ifndef BIG_NUMBERS_BIG_FLOAT_HPP
#define BIG_NUMBERS_BIG_FLOAT_HPP

#include "big_int.hpp"

template<class T>
class big_float_debugger;

template<class T>
class big_float {
private:
    friend big_float_debugger<T>;
private:
    int32_t exponent;
    big_int<T> mantissa;
public:
    explicit big_float(big_int<T> mantissa, int32_t exponent);

    std::string binary_str() const;
};

template<typename T>
big_float<T> parse_big_float(std::string source, std::size_t mantissa_width);

#endif //BIG_NUMBERS_BIG_FLOAT_HPP
