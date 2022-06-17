#ifndef BIG_NUMBERS_BIG_INT_DEBUGGER_H
#define BIG_NUMBERS_BIG_INT_DEBUGGER_H

#include <vector>
#include <cstdint>

#include "BigIntBackend.hpp"

namespace BigNumbers {
    template<class T>
    class BigIntDebugger {
    private:
        const BigIntBackend<T> &debugTarget;
    public:
        explicit BigIntDebugger(const BigIntBackend<T> &debug);

        std::vector<T> getPieces();

        uint8_t getSign();

        static BigIntBackend<T> createFromSource(const std::vector<T> &source, uint8_t sign);
    };
}
#endif //BIG_NUMBERS_BIG_INT_DEBUGGER_H
