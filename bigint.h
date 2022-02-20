#ifndef BIG_NUMBERS_BIGINT_H
#define BIG_NUMBERS_BIGINT_H

#include <vector>
#include <string>

template<typename T>
class BigInt {
private:
    char sign;
    std::vector<T> pieces;
public:
    explicit BigInt(std::vector<T> pieces);

    BigInt operator+(const BigInt<T> & a);
};

#include "bigint.cpp"

#endif //BIG_NUMBERS_BIGINT_H
