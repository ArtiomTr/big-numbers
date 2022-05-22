#ifndef BIG_NUMBERS_BIG_INT_DEBUGGER_H
#define BIG_NUMBERS_BIG_INT_DEBUGGER_H

#include <vector>
#include <cstdint>
#include "big_int.hpp"

template<class T>
class big_int_debugger {
public:
    static std::pair<std::vector<T>, uint8_t> debug(const big_int<T> &);
};

#endif //BIG_NUMBERS_BIG_INT_DEBUGGER_H
