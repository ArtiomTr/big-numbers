#ifndef BIG_NUMBERS_PADDING_HPP
#define BIG_NUMBERS_PADDING_HPP

#include <vector>

template<typename T>
class padded_list {
public:
    virtual T get(typename std::vector<T>::size_type t);
};

template<typename T>
class default_pad {
public:
    static padded_list<T> *pad_sum(const std::vector<T> &values, uint8_t sign);
};

#include "padding.cpp"

#endif //BIG_NUMBERS_PADDING_HPP
