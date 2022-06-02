#include "BigIntDebugger.h"

template<class T>
BigIntDebugger<T>::BigIntDebugger(const BigInt<T> &debug):
        debugTarget(debug) {
}

template<class T>
std::vector<T> BigIntDebugger<T>::getPieces() {
    return debugTarget.pieces;
}

template<class T>
uint8_t BigIntDebugger<T>::getSign() {
    return debugTarget.sign;
}

template
class BigIntDebugger<uint8_t>;
