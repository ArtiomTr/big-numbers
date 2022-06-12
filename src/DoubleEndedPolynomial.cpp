#include "DoubleEndedPolynomial.h"

namespace BigNumbers {

    // Insecure class. Doesn't follow any encapsulation rules, to remove redundant calls (getters/setters).
    // Shouldn't be exposed to public.
    template<class C>
    struct DoubleEndedPolynomial<C>::Node {
    public:
        Node *next;
        Node *previous;

        DoubleEndedPolynomial<C>::CoefficientType coefficient;

        explicit Node(DoubleEndedPolynomial<C>::CoefficientType coefficient);
    };

    template<class C>
    DoubleEndedPolynomial<C>::Node::Node(CoefficientType coefficient):
            coefficient(coefficient), next(nullptr), previous(nullptr) {
    }

    template<class C>
    DoubleEndedPolynomial<C>::Iterator::Iterator(DoubleEndedPolynomial<C>::Node *current,
                                                 bool endReached, bool beginReached):
            current(current), beginReached(beginReached), endReached(endReached) {

    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator &DoubleEndedPolynomial<C>::Iterator::operator++() {
        if (beginReached) {
            beginReached = false;
        } else if (current->next != nullptr) {
            current = current->next;
        } else {
            endReached = true;
        }

        return *this;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::Iterator::operator++(int) {
        Iterator returnValue = *this;
        ++(*this);
        return returnValue;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator &DoubleEndedPolynomial<C>::Iterator::operator--() {
        if (endReached) {
            endReached = false;
        } else if (current->previous != nullptr) {
            current = current->previous;
        } else {
            beginReached = true;
        }

        return *this;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::Iterator::operator--(int) {
        Iterator returnValue = *this;
        --(*this);
        return returnValue;
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::Iterator::operator==(const DoubleEndedPolynomial<C>::Iterator &other) const {
        return current == other.current && beginReached == other.beginReached && endReached == other.endReached;
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::Iterator::operator!=(const DoubleEndedPolynomial<C>::Iterator &other) const {
        return current != other.current || beginReached != other.beginReached || endReached != other.endReached;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::Iterator::operator*() {
        return current->coefficient;
    }

    template<class C>
    DoubleEndedPolynomial<C>::DoubleEndedPolynomial(): head(nullptr), tail(nullptr), size(0) {
    }

    template<class C>
    DoubleEndedPolynomial<C>::~DoubleEndedPolynomial() {
        clear();
    }

    template<class C>
    void DoubleEndedPolynomial<C>::copyFrom(const DoubleEndedPolynomial<C> &copySource) {
        clear();

        if (copySource.empty()) {
            return;
        }

        size = copySource.size;
        head = new Node(copySource.head->coefficient);
        Node *current = copySource.head->next;
        Node *previous = head;

        while (current != nullptr) {
            Node *newNode = new Node(current->coefficient);
            newNode->previous = previous;
            previous->next = newNode;

            previous = newNode;
            current = current->next;
        }

        tail = previous;
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::empty() const {
        return size == 0;
    }

    template<class C>
    DoubleEndedPolynomial<C>::DoubleEndedPolynomial(const DoubleEndedPolynomial<C> &copySource)
            : DoubleEndedPolynomial() {
        copyFrom(copySource);
    }

    template<class C>
    DoubleEndedPolynomial<C> &DoubleEndedPolynomial<C>::operator=(const DoubleEndedPolynomial<C> &copySource) {
        if (&copySource != this) {
            copyFrom(copySource);
        }

        return *this;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::clear() {
        Node *current = head;

        while (current != nullptr) {
            Node *toDelete = current;
            current = current->next;
            delete toDelete;
        }

        size = 0;
        head = nullptr;
        tail = nullptr;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::begin() const {
        return Iterator(head, false, false);
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::end() const {
        return Iterator(tail, tail != nullptr, false);
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ReverseIterator DoubleEndedPolynomial<C>::rbegin() const {
        return std::reverse_iterator(end());
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ReverseIterator DoubleEndedPolynomial<C>::rend() const {
        return std::reverse_iterator(begin());
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::SizeType DoubleEndedPolynomial<C>::getSize() const {
        return size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::insert(Iterator iterator, CoefficientType coefficient) {
        Node *newNode = new Node(coefficient);
        ++size;

        if (iterator.current != nullptr && !iterator.endReached) {
            newNode->previous = iterator.current->previous;
            iterator.current->previous = newNode;
            newNode->next = iterator.current;
        } else if (iterator.current != nullptr) {
            newNode->previous = iterator.current;
            iterator.current->next = newNode;
        }

        if (head == nullptr || (iterator.current == head && !iterator.endReached)) {
            head = newNode;
        }

        if (tail == nullptr || (iterator.current == tail && iterator.endReached)) {
            tail = newNode;
        }
    }

    template<class C>
    void DoubleEndedPolynomial<C>::insert(ReverseIterator iterator, CoefficientType coefficient) {
        insert(iterator.base(), coefficient);
    }

    template<class C>
    void DoubleEndedPolynomial<C>::pushBack(CoefficientType coefficient) {
        insert(end(), coefficient);
    }

    template<class C>
    void DoubleEndedPolynomial<C>::pushFront(CoefficientType coefficient) {
        insert(begin(), coefficient);
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::Iterator it) {
        if (size == 0) {
            throw std::logic_error("Cannot pop back - already empty polynomial.");
        }

        if (it.beginReached || it.endReached) {
            throw std::logic_error("Cannot delete element - iterator points to the end of list.");
        }

        --size;

        if (it.current->previous != nullptr) {
            it.current->previous->next = it.current->next;
        }

        if (it.current->next != nullptr) {
            it.current->next->previous = it.current->previous;
        }

        if (it.current == head) {
            head = it.current->next;
        }

        if (it.current == tail) {
            tail = it.current->previous;
        }

        delete it.current;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::ReverseIterator it) {
        erase(it.base());
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popBack() {
        erase(--end());
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popFront() {
        erase(begin());
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::front() {
        return head->coefficient;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::back() {
        return tail->coefficient;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType DoubleEndedPolynomial<C>::front() const {
        return head->coefficient;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType DoubleEndedPolynomial<C>::back() const {
        return tail->coefficient;
    }

    template<class C, class ForwardIterator>
    void trimDirection(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value,
                       ForwardIterator begin, ForwardIterator end) {
        auto it = begin;
        while (it != end && *it == value) {
            auto iteratorCopy = it;
            ++it;
            polynomial.erase(iteratorCopy);
        }
    }

    template<class C>
    void trimFront(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value) {
        trimDirection(polynomial, value, polynomial.begin(), polynomial.end());
    }

    template<class C>
    void trimBack(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value) {
        trimDirection(polynomial, value, polynomial.rbegin(), polynomial.rend());
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
