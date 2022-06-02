#ifndef BIG_NUMBERS_PARSING_UTILS_H
#define BIG_NUMBERS_PARSING_UTILS_H

#include <vector>
#include <string>

std::vector<uint8_t> decimal_string_to_numbers(const std::string &source);

template<class V>
std::vector<V> integral_source_to_binary(const std::string &source);

template<class V>
std::pair<std::vector<V>, uint32_t> fractional_source_to_binary(const std::string &source,
                                                                std::size_t width, bool shift);

#endif //BIG_NUMBERS_PARSING_UTILS_H
