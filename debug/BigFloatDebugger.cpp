#include "BigFloatDebugger.h"

template<class T>
BigFloatDebugger<T>::BigFloatDebugger(const BigFloat<T> &target)
        : debugTarget(target) {
}

template<class T>
int32_t BigFloatDebugger<T>::getExponent() {
    return debugTarget.exponent;
}

template<class T>
BigInt<T> BigFloatDebugger<T>::getMantissa() {
    return debugTarget.mantissa;
}

template
class BigFloatDebugger<uint8_t>;