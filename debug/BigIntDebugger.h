#ifndef BIG_NUMBERS_BIG_INT_DEBUGGER_H
#define BIG_NUMBERS_BIG_INT_DEBUGGER_H

#include <vector>
#include <cstdint>

#include "BigInt.hpp"

namespace BigNumbers {
    template<class T>
    class BigIntDebugger {
    private:
        const BigInt <T> &debugTarget;
    public:
        explicit BigIntDebugger(const BigInt <T> &debug);

        std::vector<T> getPieces();

        uint8_t getSign();

        static BigInt <T> createFromSource(const std::vector<T> &source, uint8_t sign);
    };
}
#endif //BIG_NUMBERS_BIG_INT_DEBUGGER_H
