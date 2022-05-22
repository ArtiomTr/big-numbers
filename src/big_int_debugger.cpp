#include "big_int_debugger.h"

template<class T>
std::pair<std::vector<T>, uint8_t> big_int_debugger<T>::debug(const big_int<T> &value) {
    return {value.pieces, value.sign};
}

template
class big_int_debugger<uint8_t>;
