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
    DoubleEndedPolynomial<C>::Iterator::Iterator(DoubleEndedPolynomial<C>::Node *current): current(current) {

    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator &DoubleEndedPolynomial<C>::Iterator::operator++() {
        if (current != nullptr) {
            current = current->next;
        }
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::Iterator::operator++(int) {
        Iterator returnValue = *this;
        ++(*this);
        return returnValue;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator &DoubleEndedPolynomial<C>::Iterator::operator--() {
        if (current != nullptr) {
            current = current->previous;
        }
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::Iterator::operator--(int) {
        Iterator returnValue = *this;
        --(*this);
        return returnValue;
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::Iterator::operator==(const DoubleEndedPolynomial<C>::Iterator &other) const {
        return current == other.current;
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::Iterator::operator!=(const DoubleEndedPolynomial<C>::Iterator &other) const {
        return current != other.current;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::Iterator::operator*() {
        return *current;
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

        head = new Node(copySource.head->coefficient);
        Node *current = copySource.head;
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
    DoubleEndedPolynomial<C>::DoubleEndedPolynomial(const DoubleEndedPolynomial<C> &copySource) :
            DoubleEndedPolynomial(copySource.startFill, copySource.endFill) {
        copyFrom(copySource);
    }

    template<class C>
    DoubleEndedPolynomial<C> &DoubleEndedPolynomial<C>::operator=(const DoubleEndedPolynomial<C> &copySource) {
        if (&copySource != *this) {
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
        return Iterator(head);
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::end() const {
        return Iterator(nullptr);
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ReverseIterator DoubleEndedPolynomial<C>::rbegin() const {
        return std::reverse_iterator(Iterator(tail));
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::ReverseIterator DoubleEndedPolynomial<C>::rend() const {
        return std::reverse_iterator(Iterator(nullptr));
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::SizeType DoubleEndedPolynomial<C>::getSize() const {
        return size;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::pushBack(CoefficientType coefficient) {
        Node *newNode = new Node(coefficient);
        ++size;

        if (tail == nullptr) {
            head = newNode;
            tail = newNode;

            return;
        }

        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::pushFront(CoefficientType coefficient) {
        Node *newNode = new Node(coefficient);
        ++size;

        if (head == nullptr) {
            head = newNode;
            tail = newNode;

            return;
        }

        head->previous = newNode;
        newNode->next = head;
        head = newNode;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::Iterator it) {
        if (size == 0) {
            throw std::logic_error("Cannot pop back - already empty polynomial.");
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
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::ReverseIterator it) {
        erase(it.base());
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popBack() {
        erase(Iterator(tail));
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
    void trimStart(DoubleEndedPolynomial<C> &polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value) {
        trimDirection(polynomial, value, polynomial.begin(), polynomial.end());
    }

    template<class C>
    void trimEnd(DoubleEndedPolynomial<C> polynomial, typename DoubleEndedPolynomial<C>::CoefficientType value) {
        trimDirection(polynomial, value, polynomial.rbegin(), polynomial.rend());
    }
}
