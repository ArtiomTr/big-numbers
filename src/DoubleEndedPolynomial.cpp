#include "DoubleEndedPolynomial.h"

namespace BigNumbers {

    template<class C>
    DoubleEndedPolynomial<C>::DoubleEndedPolynomial(): size(0) {
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::empty() const {
        return size == 0;
    }

    template<class C>
    DoubleEndedPolynomial<C>::DoubleEndedPolynomial(const DoubleEndedPolynomial<C> &copySource)
            : size(copySource.size), values(copySource.values) {
    }

    template<class C>
    DoubleEndedPolynomial<C> &DoubleEndedPolynomial<C>::operator=(const DoubleEndedPolynomial<C> &copySource) {
        if (&copySource != this) {
            size = copySource.size;
            values = copySource.values;
        }

        return *this;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::clear() {
        values.clear();
        size = 0;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::begin() {
        return values.begin();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::end() {
        return values.end();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ConstIterator DoubleEndedPolynomial<C>::begin() const {
        return values.begin();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ConstIterator DoubleEndedPolynomial<C>::end() const {
        return values.end();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ReverseIterator DoubleEndedPolynomial<C>::rbegin() {
        return values.rbegin();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ReverseIterator DoubleEndedPolynomial<C>::rend() {
        return values.rend();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ConstReverseIterator DoubleEndedPolynomial<C>::rbegin() const {
        return values.rbegin();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ConstReverseIterator DoubleEndedPolynomial<C>::rend() const {
        return values.rend();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::SizeType DoubleEndedPolynomial<C>::getSize() const {
        return size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::insert(Iterator iterator, CoefficientType coefficient) {
        values.insert(iterator, coefficient);
        ++size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::insert(ReverseIterator iterator, CoefficientType coefficient) {
        insert(std::prev(iterator.base()), coefficient);
    }

    template<class C>
    void DoubleEndedPolynomial<C>::pushBack(CoefficientType coefficient) {
        values.push_back(coefficient);
        ++size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::pushFront(CoefficientType coefficient) {
        values.push_front(coefficient);
        ++size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::Iterator it) {
        if (size == 0) {
            throw std::logic_error("Cannot erase element - already empty polynomial.");
        }

        values.erase(it);
        --size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::ReverseIterator it) {
        erase(std::prev(it.base()));
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popBack() {
        values.pop_back();
        --size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popFront() {
        values.pop_front();
        --size;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::front() {
        return values.front();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::back() {
        return values.back();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType DoubleEndedPolynomial<C>::front() const {
        return values.front();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType DoubleEndedPolynomial<C>::back() const {
        return values.back();
    }

    template<class C>
    void trimFront(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value) {
        while (!polynomial.empty() && polynomial.front() == value) {
            polynomial.popFront();
        }
    }

    template<class C>
    void trimBack(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value) {
        while (!polynomial.empty() && polynomial.back() == value) {
            polynomial.popBack();
        }
    }

    template<class C, class ForwardIterator>
    void extendDirection(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::SizeType count,
                         typename DoubleEndedPolynomial<C>::CoefficientType value, ForwardIterator position) {
        for (std::size_t i = 0; i < count; ++i) {
            polynomial.insert(position, value);
            --position;
        }
    }

    template<class C>
    void extendFront(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::SizeType count,
                     typename DoubleEndedPolynomial<C>::CoefficientType value) {
        extendDirection(polynomial, count, value, polynomial.begin());
    }

    template<class C>
    void extendBack(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::SizeType count,
                    typename DoubleEndedPolynomial<C>::CoefficientType value) {
        extendDirection(polynomial, count, value, polynomial.rbegin());
    }

    template
    class DoubleEndedPolynomial<uint8_t>;

    template
    void trimFront(DoubleEndedPolynomial<uint8_t> &polynomial,
                   typename DoubleEndedPolynomial<uint8_t>::CoefficientType value);

    template
    void trimBack(DoubleEndedPolynomial<uint8_t> &polynomial,
                  typename DoubleEndedPolynomial<uint8_t>::CoefficientType value);

    template
    void extendFront(DoubleEndedPolynomial<uint8_t> &polynomial,
                     typename DoubleEndedPolynomial<uint8_t>::SizeType count,
                     typename DoubleEndedPolynomial<uint8_t>::CoefficientType value);

    template
    void extendBack(DoubleEndedPolynomial<uint8_t> &polynomial, typename DoubleEndedPolynomial<uint8_t>::SizeType count,
                    typename DoubleEndedPolynomial<uint8_t>::CoefficientType value);
}
