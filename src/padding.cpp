#include "padding.hpp"

#include <stdexcept>
#include <limits>

template<typename T>
T padded_list<T>::get(typename std::vector<T>::size_type t) {
    throw std::logic_error("Method is not implemented");
}

template<typename T>
class left_padded_list : public padded_list<T> {
private:
    const std::vector<T> &values;
    const T padValue;
public:
    left_padded_list(const std::vector<T> &values, T padValue): values(values), padValue(padValue) {

    }

    T get(typename std::vector<T>::size_type index) override {
        if(index >= values.size()) {
            return padValue;
        }

        return values[index];
    }
};

template<typename T>
padded_list<T> *default_pad<T>::pad_sum(const std::vector<T> &values, uint8_t sign) {
    padded_list<T> *list = new left_padded_list<T>(values, sign ? std::numeric_limits<T>::max() : 0b0);

    return list;
}

template
class default_pad<uint8_t>;