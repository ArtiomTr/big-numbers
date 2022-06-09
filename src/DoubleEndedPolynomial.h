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
            friend class DoubleEndedPolynomial<C>;

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
