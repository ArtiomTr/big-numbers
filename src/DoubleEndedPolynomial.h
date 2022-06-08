#ifndef BIG_NUMBERS_DOUBLEENDEDPOLYNOMIAL_H
#define BIG_NUMBERS_DOUBLEENDEDPOLYNOMIAL_H

#include <cstddef>
#include <iterator>

namespace BigNumbers {

    // Abstract data type, that is used to efficiently save polynomials of arbitrary size.
    template<class C>
    class DoubleEndedPolynomial {
    public:
        using CoefficientType = C;
        using SizeType = std::size_t;
    private:
        struct Node;
    public:
        class Iterator : public std::iterator<std::bidirectional_iterator_tag, CoefficientType> {
        private:
            Node *current;
        public:
            explicit Iterator(DoubleEndedPolynomial<C>::Node *current);

            Iterator &operator++();

            Iterator operator++(int);

            Iterator &operator--();

            Iterator operator--(int);

            bool operator==(const Iterator &other) const;

            bool operator!=(const Iterator &other) const;

            CoefficientType &operator*();
        };

        using ReverseIterator = std::reverse_iterator<Iterator>;
    private:
        Node *head;
        Node *tail;
        SizeType size;

        void copyFrom(const DoubleEndedPolynomial<C> &copySource);

    public:
        DoubleEndedPolynomial();

        ~DoubleEndedPolynomial();

        DoubleEndedPolynomial(const DoubleEndedPolynomial<C> &copySource);

        Iterator begin() const;

        Iterator end() const;

        ReverseIterator rbegin() const;

        ReverseIterator rend() const;

        [[nodiscard]] SizeType getSize() const;

        DoubleEndedPolynomial<C> &operator=(const DoubleEndedPolynomial<C> &copySource);

        void clear();

        [[nodiscard]] bool empty() const;

        void pushBack(CoefficientType coefficient);

        void pushFront(CoefficientType coefficient);

        void popBack();

        void popFront();
    };

    template<class C>
    void trimStart(DoubleEndedPolynomial<C> polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value);

    template<class C>
    void trimEnd(DoubleEndedPolynomial<C> polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value);
}

#endif //BIG_NUMBERS_DOUBLEENDEDPOLYNOMIAL_H
