#ifndef BIG_NUMBERS_PARSINGUTILS_H
#define BIG_NUMBERS_PARSINGUTILS_H

#include <string>

#include "BigIntBackend.h"
#include "BigFloatBackend.h"

namespace BigNumbers {
    template<class V>
    BigIntBackend<V> parseBigInt(std::string source);

    template<typename T>
    BigFloatBackend<T> parseBigFloat(std::string source, std::size_t mantissaWidth);
}

#endif //BIG_NUMBERS_PARSINGUTILS_H
