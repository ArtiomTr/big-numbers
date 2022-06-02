#ifndef BIG_NUMBERS_PADDING_HPP
#define BIG_NUMBERS_PADDING_HPP

#include <vector>
#include <cstdint>

template<class T>
class PaddedList {
public:
    virtual T get(typename std::vector<T>::size_type t);
};

template<class T>
class Pad {
public:
    virtual PaddedList<T> *padForSum(const std::vector<T> &values, uint8_t sign);
};

template<class T>
class DefaultPad : public Pad<T> {
public:
    PaddedList<T> *padForSum(const std::vector<T> &values, uint8_t sign) override;
};

#endif //BIG_NUMBERS_PADDING_HPP
