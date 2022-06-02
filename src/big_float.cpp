#include "big_float.hpp"

#include <regex>
#include <string>
#include <iostream>
#include <list>

#include "parsing_utils.h"

template<class T>
big_float<T>::big_float(big_int<T> mantissa, int32_t exponent): exponent(exponent), mantissa(mantissa) {
}

template<class T>
std::string big_float<T>::binary_str() const {
    std::stringstream out{};
    out << mantissa.binary_str() << "E" << exponent;

    return out.str();
}

template<class V>
std::pair<std::vector<V>, int32_t> fractional_source_to_binary(
) {

}

template<typename T>
big_float<T> parse_big_float(std::string source, std::size_t mantissa_width) {
    std::regex big_float_regex(R"(^-?\d+\.\d+$)");

    if (!std::regex_match(source, big_float_regex)) {
        throw std::invalid_argument("Invalid big_float format");
    }

    typedef typename std::vector<T>::size_type size_type;

    uint8_t sign = source[0] == '-';

    if (sign) {
        source.erase(source.begin());
    }

    std::string::size_type dot_position = source.find('.');
    std::vector<T> integralPart = integral_source_to_binary<T>(source.substr(0, dot_position));

    std::size_t end;
    for (end = integralPart.size(); end > 0; --end) {
        if (integralPart[end - 1] != 0b0) {
            break;
        }
    }

    integralPart.erase(integralPart.begin() + end, integralPart.end());
    int32_t exponent = std::max((int) integralPart.size() - 1, 0);

    if (integralPart.size() > mantissa_width) {
        throw std::logic_error("Too small precision: unsafe integer bound exceeded, precision is less than 1 unit.");
    }

    if (integralPart.size() < mantissa_width) {
        auto fractionPart = fractional_source_to_binary<T>(source.substr(dot_position + 1),
                                                           mantissa_width - integralPart.size(),
                                                           integralPart.empty());

        integralPart.insert(integralPart.end(), fractionPart.first.begin(), fractionPart.first.end());

        exponent += fractionPart.second;
    }

    std::reverse(integralPart.begin(), integralPart.end());
    big_int<T> mantissa(integralPart, 0);

    return big_float<T>(mantissa, exponent);
}

template
class big_float<uint8_t>;

template big_float<uint8_t> parse_big_float(std::string source, std::size_t mantissa_width);