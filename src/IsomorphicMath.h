#ifndef BIG_NUMBERS_ISOMORPHICMATH_H
#define BIG_NUMBERS_ISOMORPHICMATH_H

#include <cstddef>

namespace IsomorphicMath {
    template<class T>
    T sqrt(T value, T epsilon) {
        T x = value;
        T y = 1;
        T two = 2;

        while ((x - y) > epsilon) {
            x += y;
            x /= two;
            y = value / x;
        }

        return x;
    }

    template<class T>
    bool isPrime(T value) {
        if (value == 0 || value == 1) {
            return false;
        }

        if (value == 2) {
            return true;
        }

        T squareRoot = sqrt(value, (T) 0);
        for (T i = 2; i <= squareRoot; ++i) {
            if (value % i == 0) {
                return false;
            }
        }

        return true;
    }

    template<class T>
    T findNextPrime(T value) {
        ++value;
        while (!isPrime(value))
            ++value;
        return value;
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
