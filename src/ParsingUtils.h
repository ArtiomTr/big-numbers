#ifndef BIG_NUMBERS_PARSINGUTILS_H
#define BIG_NUMBERS_PARSINGUTILS_H

#include <string>

#include "BigIntBackend.hpp"
#include "BigFloatBackend.hpp"

namespace BigNumbers {
    template<class T>
    BigIntBackend<T> parseBigInt(std::string source);

    template<typename T>
    BigFloatBackend<T> parseBigFloat(std::string source, std::size_t mantissaWidth);
}

#endif //BIG_NUMBERS_PARSINGUTILS_H
