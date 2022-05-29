#ifndef BIG_NUMBERS_BIG_INT_DEBUGGER_H
#define BIG_NUMBERS_BIG_INT_DEBUGGER_H

#include <vector>
#include <cstdint>

#include "big_int.hpp"

template<class T>
class big_int_debugger {
private:
    const big_int<T> &debug_target;
public:
    explicit big_int_debugger(const big_int<T> &debug);

    std::vector<T> get_pieces();

    uint8_t get_sign();
};

#endif //BIG_NUMBERS_BIG_INT_DEBUGGER_H
