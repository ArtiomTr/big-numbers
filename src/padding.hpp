#ifndef BIG_NUMBERS_PADDING_HPP
#define BIG_NUMBERS_PADDING_HPP

#include <vector>
#include <cstdint>

template<class T>
class padded_list {
public:
    virtual T get(typename std::vector<T>::size_type t);
};

template<class T>
class pad {
public:
    virtual padded_list<T> *pad_for_sum(const std::vector<T> &values, uint8_t sign);
};

template<class T>
class default_pad : public pad<T> {
public:
    padded_list<T> *pad_for_sum(const std::vector<T> &values, uint8_t sign) override;
};

#endif //BIG_NUMBERS_PADDING_HPP
