#include "BigFloatDebugger.h"

namespace BigNumbers {
    template<class T>
    BigFloatDebugger<T>::BigFloatDebugger(const BigFloatBackend<T> &target)
            : debugTarget(target) {
    }

    template<class T>
    int32_t BigFloatDebugger<T>::getExponent() {
        return debugTarget.exponent;
    }

    template<class T>
    BigIntBackend<T> BigFloatDebugger<T>::getMantissa() {
        return debugTarget.mantissa;
    }

    template
    class BigFloatDebugger<uint8_t>;
}