#ifndef BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H
#define BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H

#include "BigFloat.hpp"

namespace BigNumbers {
    template<class T>
    class BigFloatDebugger {
    private:
        const BigFloat<T> &debugTarget;
    public:
        explicit BigFloatDebugger(const BigFloat<T> &target);

        int32_t getExponent();

        BigIntBackend<T> getMantissa();
    };
}
#endif //BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H
