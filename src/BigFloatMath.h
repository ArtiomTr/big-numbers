#ifndef BIG_NUMBERS_BIGFLOATMATH_H
#define BIG_NUMBERS_BIGFLOATMATH_H

#include "BigFloat.h"

namespace BigNumbers {
    BigFloat sin(BigFloat value);

    BigFloat sqrt(const BigFloat &value);

    BigFloat findNextPrime(const BigFloat &value);

    BigFloat factorial(std::size_t n);

    BigFloat pow(const BigFloat &value, int power);

    BigFloat ln(BigFloat value);

    BigFloat pi(int digitsAfterDot);
}

#endif //BIG_NUMBERS_BIGFLOATMATH_H
