#ifndef BIG_NUMBERS_PARSINGUTILS_H
#define BIG_NUMBERS_PARSINGUTILS_H

#include <string>

#include "BigInt.hpp"
#include "BigFloat.hpp"

namespace BigNumbers {
    template<class T>
    BigInt<T> parseBigInt(std::string source);

    template<typename T>
    BigFloat<T> parseBigFloat(std::string source, std::size_t mantissaWidth);
}

#endif //BIG_NUMBERS_PARSINGUTILS_H
