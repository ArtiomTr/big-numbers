#ifndef BIG_NUMBERS_DOUBLEENDEDPOLYNOMIAL_H
#define BIG_NUMBERS_DOUBLEENDEDPOLYNOMIAL_H

#include <cstddef>
#include <iterator>
#include <list>

namespace BigNumbers {

    // Abstract data type, that is used to efficiently save polynomials of arbitrary size.
    template<class C>
    class DoubleEndedPolynomial {
    public:
        using CoefficientType = C;
        using SizeType = std::size_t;
        using Iterator = typename std::list<C>::iterator;
        using ConstIterator = typename std::list<C>::const_iterator;
        using ReverseIterator = typename std::list<C>::reverse_iterator;
        using ConstReverseIterator = typename std::list<C>::const_reverse_iterator;
    private:
        SizeType size;

        std::list<C> values;
    public:
        DoubleEndedPolynomial();

        DoubleEndedPolynomial(const DoubleEndedPolynomial<C> &copySource);

        Iterator begin();

        Iterator end();

        ConstIterator begin() const;

        ConstIterator end() const;

        ReverseIterator rbegin();

        ReverseIterator rend();

        ConstReverseIterator rbegin() const;

        ConstReverseIterator rend() const;

        [[nodiscard]] SizeType getSize() const;

        DoubleEndedPolynomial<C> &operator=(const DoubleEndedPolynomial<C> &copySource);

        void clear();

        [[nodiscard]] bool empty() const;

        void insert(Iterator iterator, CoefficientType coefficient);

        void insert(ReverseIterator iterator, CoefficientType coefficient);

        void pushBack(CoefficientType coefficient);

        void pushFront(CoefficientType coefficient);

        void erase(Iterator it);

        void erase(ReverseIterator it);

        void popBack();

        void popFront();

        CoefficientType &front();

        CoefficientType front() const;

        CoefficientType &back();

        CoefficientType back() const;
    };

    template<class C>
    void trimFront(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value);

    template<class C>
    void trimBack(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value);

    template<class C>
    void extendFront(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::SizeType count,
                     typename DoubleEndedPolynomial<C>::CoefficientType value);

    template<class C>
    void extendBack(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::SizeType count,
                    typename DoubleEndedPolynomial<C>::CoefficientType value);
}

#endif //BIG_NUMBERS_DOUBLEENDEDPOLYNOMIAL_H
