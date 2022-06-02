#include "Padding.hpp"

#include <stdexcept>
#include <limits>

template<class T>
T PaddedList<T>::get(typename std::vector<T>::size_type t) {
    throw std::logic_error("Method is not implemented");
}

template<class T>
class LeftPaddedList : public PaddedList<T> {
private:
    const std::vector<T> &values;
    const T padValue;
public:
    LeftPaddedList(const std::vector<T> &values, T padValue) : values(values), padValue(padValue) {

    }

    T get(typename std::vector<T>::size_type index) override {
        if (index >= values.size()) {
            return padValue;
        }

        return values[index];
    }
};

template<class T>
PaddedList<T> *Pad<T>::padForSum(const std::vector<T> &values, uint8_t sign) {
    throw std::logic_error("Method is not implemented");
}

template<class T>
PaddedList<T> *DefaultPad<T>::padForSum(const std::vector<T> &values, uint8_t sign) {
    PaddedList<T> *list = new LeftPaddedList<T>(values, sign ? std::numeric_limits<T>::max() : 0b0);

    return list;
}

template
class Pad<uint8_t>;

template
class DefaultPad<uint8_t>;
