#ifndef BIG_NUMBERS_PARSING_UTILS_H
#define BIG_NUMBERS_PARSING_UTILS_H

#include <vector>
#include <string>

std::vector<uint8_t> decimal_string_to_numbers(const std::string &source);

std::vector<uint8_t> integral_source_to_binary(const std::string &source);

#endif //BIG_NUMBERS_PARSING_UTILS_H
