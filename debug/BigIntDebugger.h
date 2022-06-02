#ifndef BIG_NUMBERS_BIG_INT_DEBUGGER_H
#define BIG_NUMBERS_BIG_INT_DEBUGGER_H

#include <vector>
#include <cstdint>

#include "BigInt.hpp"

template<class T>
class BigIntDebugger {
private:
    const BigInt<T> &debugTarget;
public:
    explicit BigIntDebugger(const BigInt<T> &debug);

    std::vector<T> getPieces();

    uint8_t getSign();
};

#endif //BIG_NUMBERS_BIG_INT_DEBUGGER_H
