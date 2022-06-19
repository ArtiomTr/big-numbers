#ifndef BIG_NUMBERS_VECTORUTILS_H
#define BIG_NUMBERS_VECTORUTILS_H

#include <vector>

namespace BigNumbers {

    template<class V>
    void trimFront(std::vector<V> &in, V value) {
        typename std::vector<V>::iterator newBeginning;
        for (newBeginning = in.begin(); newBeginning != in.end(); ++newBeginning) {
            if (*newBeginning != value) {
                break;
            }
        }

        in.erase(in.begin(), newBeginning);
    }

    template<class Container>
    void trimBack(Container &in, typename Container::value_type value) {
        typename Container::iterator newEnd;
        for (newEnd = in.end(); newEnd != in.begin(); --newEnd) {
            if (*std::prev(newEnd) != value) {
                break;
            }
        }

        in.erase(newEnd, in.end());
    }

    template<class Container>
    void extendFront(Container &in, typename Container::value_type value, int64_t count) {
        if (count < 0) {
            throw std::logic_error("Cannot extend container with negative amount of items.");
        }

        if (count == 0) {
            return;
        }

        in.insert(in.begin(), count, value);
    }

    template<class V>
    void extendBack(std::vector<V> &in, V value, int64_t count) {
        if (count < 0) {
            throw std::logic_error("Cannot extend container with negative amount of items.");
        }

        if (count == 0) {
            return;
        }

        in.insert(in.end(), count, value);
    }
}

#endif //BIG_NUMBERS_VECTORUTILS_H
