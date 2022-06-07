#ifndef BIG_NUMBERS_ISOMORPHICMATH_HPP
#define BIG_NUMBERS_ISOMORPHICMATH_HPP

namespace IsomorphicMath {
    template<class T>
    T abs(const T &value) {
        return value < 0 ? -value : value;
    }

    template<class T>
    T delta(const T &first, const T &second) {
        if (first > second) {
            return first - second;
        } else {
            return second - first;
        }
    }
}

#endif //BIG_NUMBERS_ISOMORPHICMATH_HPP
