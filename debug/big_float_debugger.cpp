#include "big_float_debugger.h"

template<class T>
big_float_debugger<T>::big_float_debugger(const big_float<T> &target)
        : debug_target(target) {
}

template<class T>
int32_t big_float_debugger<T>::get_exponent() {
    return debug_target.exponent;
}

template<class T>
big_int<T> big_float_debugger<T>::get_mantissa() {
    return debug_target.mantissa;
}

template
class big_float_debugger<uint8_t>;