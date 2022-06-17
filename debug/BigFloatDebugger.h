#ifndef BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H
#define BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H

#include "BigFloatBackend.h"

namespace BigNumbers {
    template<class T>
    class BigFloatDebugger {
    private:
        const BigFloatBackend<T> &debugTarget;
    public:
        explicit BigFloatDebugger(const BigFloatBackend<T> &target);

        int32_t getExponent();

        BigIntBackend<T> getMantissa();
    };
}
#endif //BIG_NUMBERS_BIG_FLOAT_DEBUGGER_H
