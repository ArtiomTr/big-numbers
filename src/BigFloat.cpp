#include "BigFloat.hpp"

#include <regex>
#include <string>
#include <iostream>

#include "ParsingUtils.h"

template<class T>
BigFloat<T>::BigFloat(BigInt<T> mantissa, int32_t exponent): exponent(exponent), mantissa(mantissa) {
}

template<class T>
std::string BigFloat<T>::toString() const {
    std::stringstream out{};
    out << mantissa.toString() << "E" << exponent;

    return out.str();
}

template<typename T>
BigFloat<T> parseBigFloat(std::string source, std::size_t mantissaWidth) {
    std::regex bigFloatRegex(R"(^-?\d+\.\d+$)");

    if (!std::regex_match(source, bigFloatRegex)) {
        throw std::invalid_argument("Invalid BigFloat format");
    }

    uint8_t sign = source[0] == '-';

    if (sign) {
        source.erase(source.begin());
    }

    std::string::size_type dotPosition = source.find('.');
    std::vector<T> integralPart = integralSourceToBinary<T>(source.substr(0, dotPosition));

    std::size_t end;
    for (end = integralPart.size(); end > 0; --end) {
        if (integralPart[end - 1] != 0b0) {
            break;
        }
    }

    integralPart.erase(integralPart.begin() + end, integralPart.end());
    int32_t exponent = std::max((int) integralPart.size() - 1, 0);

    if (integralPart.size() > mantissaWidth) {
        throw std::logic_error("Too small precision: unsafe integer bound exceeded, precision is less than 1 unit.");
    }

    if (integralPart.size() < mantissaWidth) {
        auto fractionPart = fractionalSourceToBinary<T>(source.substr(dotPosition + 1),
                                                        mantissaWidth - integralPart.size(),
                                                        integralPart.empty());

        integralPart.insert(integralPart.end(), fractionPart.first.begin(), fractionPart.first.end());

        exponent += fractionPart.second;
    }

    std::reverse(integralPart.begin(), integralPart.end());
    BigInt<T> mantissa(integralPart, 0);

    return BigFloat<T>(sign ? -mantissa : mantissa, exponent);
}

template
class BigFloat<uint8_t>;

template BigFloat<uint8_t> parseBigFloat(std::string source, std::size_t mantissaWidth);