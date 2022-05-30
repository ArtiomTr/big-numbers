#ifndef BIG_NUMBERS_ISOMORPHICMATH_HPP
#define BIG_NUMBERS_ISOMORPHICMATH_HPP

namespace IsomorphicMath {
    template<class T>
    T abs(const T &value) {
        return value < 0 ? -value : value;
    }
}

#endif //BIG_NUMBERS_ISOMORPHICMATH_HPP
