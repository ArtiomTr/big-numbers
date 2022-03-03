#include "bigint.h"

#include <limits>

template<typename T>
BigInt<T>::BigInt(std::vector<T> initialPieces, uint8_t initialSign): pieces(std::move(initialPieces)), sign(initialSign) {
    if(initialSign != 0 && initialSign != 1) {
        throw std::invalid_argument("Sign should be 0 for positive numbers or 1 for negative");
    }
}

template<typename T>
BigInt<T> BigInt<T>::operator+(const BigInt<T> & a) {
    T carry = 0;

    BigInt<T> out({}, a.sign);

    typename std::vector<T>::size_type minLength = std::min(a.pieces.size(), pieces.size());
    BigInt<T> longestBigInt = a.pieces.size() > pieces.size() ? a : *this;
    BigInt<T> shortestBigInt = a.pieces.size() < pieces.size() ? a : *this;
    typename std::vector<T>::size_type maxLength = longestBigInt.pieces.size();

    for(typename std::vector<T>::size_type i = 0; i < maxLength; ++i) {
        T value;

        if(i >= minLength) {
            value = longestBigInt.pieces[i] + (shortestBigInt.sign * std::numeric_limits<T>::max()) + carry;
        } else {
            value = a.pieces[i] + pieces[i] + carry;
        }

        if(longestBigInt.pieces[i] > value) {
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
