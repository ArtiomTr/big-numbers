#ifndef BIG_NUMBERS_PARSINGUTILS_H
#define BIG_NUMBERS_PARSINGUTILS_H

#include <string>

#include "BigInt.h"
#include "BigFloatBackend.h"

namespace BigNumbers {
    BigInt parseBigInt(std::string source);

    template<typename T>
    BigFloatBackend<T> parseBigFloat(std::string source, std::size_t mantissaWidth);
}

#endif //BIG_NUMBERS_PARSINGUTILS_H
