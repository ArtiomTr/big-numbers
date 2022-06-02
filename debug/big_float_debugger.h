#ifndef BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H
#define BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H

#include "big_float.hpp"

template<class T>
class big_float_debugger {
private:
    const big_float<T> &debug_target;
public:
    explicit big_float_debugger(const big_float<T> &target);

    int32_t get_exponent();

    big_int<T> get_mantissa();
};

#endif //BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H
