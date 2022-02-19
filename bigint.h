#ifndef BIG_NUMBERS_BIGINT_H
#define BIG_NUMBERS_BIGINT_H

#include <vector>

template<typename T>
class BigInt {
private:
    std::vector<T> pieces;
public:
    explicit BigInt(std::vector<T> pieces);
    BigInt<T> operator+(BigInt<T> a);
};

#endif //BIG_NUMBERS_BIGINT_H
