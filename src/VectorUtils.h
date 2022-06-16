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

    template<class V>
    void trimBack(std::vector<V> &in, V value) {
        typename std::vector<V>::iterator newEnd;
        for (newEnd = in.end(); newEnd != in.begin(); --newEnd) {
            if (*std::prev(newEnd) != value) {
                break;
            }
        }

        in.erase(newEnd, in.end());
    }
}

#endif //BIG_NUMBERS_VECTORUTILS_H
