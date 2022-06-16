#include "BigIntDebugger.h"

namespace BigNumbers {
    template<class T>
    BigIntDebugger<T>::BigIntDebugger(const BigInt<T> &debug):
            debugTarget(debug) {
    }

    template<class T>
    std::vector<T> BigIntDebugger<T>::getPieces() {
        return {debugTarget.pieces.begin(), debugTarget.pieces.end()};
    }

    template<class T>
    uint8_t BigIntDebugger<T>::getSign() {
        return debugTarget.sign;
    }

    template<class T>
    BigInt<T> BigIntDebugger<T>::createFromSource(const std::vector<T> &source, uint8_t sign) {
        BigInt<T> output;

        output.sign = sign;
        for (T value: source) {
            output.pieces.push_back(value);
        }

        return output;
    }

    template
    class BigIntDebugger<uint8_t>;
}