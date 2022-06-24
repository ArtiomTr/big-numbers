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
        for (std::size_t i = 2; i <= n; ++i) {
            value *= (T) i;
        }

        return value;
    }

    template<class T>
    T pow(T value, int power) {
        if (power < 0) {
            throw std::logic_error("Cannot raise value to a negative power.");
        }

        if (power == 0) {
            return 1;
        }

        T multiplier = value;
        for (int i = 1; i < power; ++i) {
            value *= multiplier;
        }

        return value;
    }

    template<class T>
    T ln(T value, bool approximate = false) {
        if (value <= 0) {
            throw std::logic_error("Cannot compute natural logarithm of non-negative number.");
        }

        if (!approximate && (value < 0.5 || value > 1)) {
            throw std::logic_error(
                    "Natural logarithm computation function gives reasonable precision in interval [0.5, 1].");
        }

        T one = 1;
        T two = 2;

        T alpha = (value - one) / (value + one);
        T answer = alpha;
        T save = answer * alpha * alpha;

        for (int i = 2; i <= 103; ++i) {
            answer += one / (two * static_cast<T>(i) - one) * save;
            save *= alpha;
            save *= alpha;
        }

        answer *= two;
        return answer;
    }

    template<class T>
    T pi(T epsilon) {
        T linearTerm = 13591409;
        T exponentialTerm = 1;
        T multinomialTerm = 1;
        T additionalTerm = -6;

        T twelveConstant = 12;
        T sixteenConstant = 16;
        T linearTermConstant = 545140134;
        T exponentialTermConstant = -262537412640768000;

        T totalSum = 0;

        for (int i = 1; i < 150; ++i) {
            totalSum += (multinomialTerm * linearTerm) / exponentialTerm;
            linearTerm += linearTermConstant;
            exponentialTerm *= exponentialTermConstant;
            additionalTerm += twelveConstant;
            multinomialTerm *=
                    (additionalTerm * additionalTerm * additionalTerm - sixteenConstant * additionalTerm) /
                    static_cast<T>(i * i * i);
        }

        T constant = static_cast<T>(426880) * sqrt(static_cast<T>(10005), epsilon);
        T inverted = constant / totalSum;

        return inverted;
    }
}

#endif //BIG_NUMBERS_ISOMORPHICMATH_H
