#include "bigint.h"

#include <sstream>

template<typename T>
BigInt<T>::BigInt(std::vector<T> initialPieces): pieces(std::move(initialPieces)), sign(1) {

}

template<typename T>
BigInt<T> BigInt<T>::operator+(const BigInt<T> & a) {
    T carry = 0;

    BigInt out({});

    typename std::vector<T>::size_type minLength = std::min(a.pieces.size(), pieces.size());
    std::vector<T> longestVector = a.pieces.size() > pieces.size() ? a.pieces : pieces;
    typename std::vector<T>::size_type maxLength = longestVector.size();

    for(typename std::vector<T>::size_type i = 0; i < maxLength; ++i) {
        T value;

        if(i >= minLength) {
            value = longestVector[i] + carry;
        } else {
            value = a.pieces[i] + pieces[i] + carry;
        }

        if(longestVector[i] > value) {
            carry = 1;
        } else {
            carry = 0;
        }

        out.pieces.push_back(value);
    }

    if(carry == 1) {
        out.pieces.push_back(carry);
    }

    return out;
}
