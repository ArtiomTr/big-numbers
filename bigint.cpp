#include "bigint.h"

template<typename T>
BigInt<T>::BigInt(std::vector<T> initialPieces): pieces(initialPieces) {

}

template<typename T>
BigInt<T> operator+(BigInt<T> a) {
    return a;
}