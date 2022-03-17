#include "padding.hpp"

#include <stdexcept>
#include <limits>

template<class T>
T padded_list<T>::get(typename std::vector<T>::size_type t) {
    throw std::logic_error("Method is not implemented");
}

template<class T>
class left_padded_list : public padded_list<T> {
private:
    const std::vector<T> &values;
    const T padValue;
public:
    left_padded_list(const std::vector<T> &values, T padValue) : values(values), padValue(padValue) {

    }

    T get(typename std::vector<T>::size_type index) override {
        if (index >= values.size()) {
            return padValue;
        }

        return values[index];
    }
};

template<class T>
padded_list<T> *pad<T>::pad_for_sum(const std::vector<T> &values, uint8_t sign) {
    throw std::logic_error("Method is not implemented");
}

template<class T>
padded_list<T> *default_pad<T>::pad_for_sum(const std::vector<T> &values, uint8_t sign) {
    padded_list<T> *list = new left_padded_list<T>(values, sign ? std::numeric_limits<T>::max() : 0b0);

    return list;
}

template
class pad<uint8_t>;

template
class default_pad<uint8_t>;
