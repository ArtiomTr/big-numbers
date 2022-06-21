#ifndef BIG_NUMBERS_ISOMORPHICMATH_H
#define BIG_NUMBERS_ISOMORPHICMATH_H

#include <cstddef>

namespace IsomorphicMath {
    template<class T>
    T sqrt(T value, T epsilon) {
        T x = value;
        T y = 1;

        std::size_t i = 0;
        while ((x - y) > epsilon) {
            x += y;
            x /= 2;
            y = value / x;
            ++i;
        }

        return x;
    }

    template<class T>
    T delta(const T &first, const T &second) {
        if (first > second) {
            return first - second;
        } else {
            return second - first;
        }
    }

    template<class T>
    T factorial(std::size_t n) {
        T value = (T) 1;
        for (std::size_t i = 1; i <= n; ++i) {
            value *= (T) i;
        }

        return value;
    }
}

#endif //BIG_NUMBERS_ISOMORPHICMATH_H
