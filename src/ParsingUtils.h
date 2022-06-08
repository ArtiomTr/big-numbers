#ifndef BIG_NUMBERS_PARSINGUTILS_H
#define BIG_NUMBERS_PARSINGUTILS_H

#include <vector>
#include <string>

#include "BigInt.hpp"
#include "BigFloat.hpp"

namespace BigNumbers {
//    std::vector<uint8_t> decimalStringToNumbers(const std::string &source);
//
//    template<class V>
//    std::vector<V> integralSourceToBinary(const std::string &source);
//
//    template<class V>
//    std::pair<std::vector<V>, uint32_t> fractionalSourceToBinary(const std::string &source,
//                                                                 std::size_t width, bool shift);

    template<class T>
    BigInt<T> parseBigInt(std::string source);

    template<typename T>
    BigFloat<T> parseBigFloat(std::string source, std::size_t mantissaWidth);
}
#endif //BIG_NUMBERS_PARSINGUTILS_H
