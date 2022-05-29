#include "big_int_debugger.h"

template<class T>
big_int_debugger<T>::big_int_debugger(const big_int<T> &debug):
        debug_target(debug) {
}

template<class T>
std::vector<T> big_int_debugger<T>::get_pieces() {
    return debug_target.pieces;
}

template<class T>
uint8_t big_int_debugger<T>::get_sign() {
    return debug_target.sign;
}

template
class big_int_debugger<uint8_t>;
