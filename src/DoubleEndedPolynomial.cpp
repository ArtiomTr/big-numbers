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

        explicit Node();

        explicit Node(DoubleEndedPolynomial<C>::CoefficientType coefficient);

        ~Node();
    };

    template<class C>
    DoubleEndedPolynomial<C>::Node::Node(): coefficient(0), next(this), previous(this) {
    }

    template<class C>
    DoubleEndedPolynomial<C>::Node::Node(CoefficientType coefficient):
            coefficient(coefficient), next(this), previous(this) {
    }

    template<class C>
    DoubleEndedPolynomial<C>::Node::~Node() {
        next->previous = previous;
        previous->next = next;
    }

    template<class C>
    DoubleEndedPolynomial<C>::Iterator::Iterator(DoubleEndedPolynomial<C>::Node *current): current(current) {

    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator &DoubleEndedPolynomial<C>::Iterator::operator++() {
        current = current->next;

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
        current = current->previous;

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
        return current == other.current;
    }

    template<class C>
    bool DoubleEndedPolynomial<C>::Iterator::operator!=(const DoubleEndedPolynomial<C>::Iterator &other) const {
        return current != other.current;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::Iterator::operator*() {
        return current->coefficient;
    }

    template<class C>
    DoubleEndedPolynomial<C>::DoubleEndedPolynomial(): size(0), header(new Node()) {
    }

    template<class C>
    DoubleEndedPolynomial<C>::~DoubleEndedPolynomial() {
        clear();
        delete header;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::copyFrom(const DoubleEndedPolynomial<C> &copySource) {
        clear();

        if (copySource.empty()) {
            return;
        }

        size = copySource.size;
        header = new Node();

        Node *current = copySource.header->next;
        Node *previous = header;

        while (current != copySource.header) {
            Node *copiedNode = new Node(current->coefficient);
            copiedNode->previous = previous;
            previous->next = copiedNode;

            previous = copiedNode;
            current = current->next;
        }

        previous->next = header;
        header->previous = previous;
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
        Node *current = header->next;

        while (current != header) {
            Node *toDelete = current;
            current = current->next;
            delete toDelete;
        }

        size = 0;
        header->previous = header;
        header->next = header;
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::begin() const {
        return Iterator(header->next);
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::Iterator DoubleEndedPolynomial<C>::end() const {
        return Iterator(header);
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

        newNode->next = iterator.current;
        newNode->previous = iterator.current->previous;
        iterator.current->previous->next = newNode;
        iterator.current->previous = newNode;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::insert(ReverseIterator iterator, CoefficientType coefficient) {
        insert(std::prev(iterator.base()), coefficient);
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
            throw std::logic_error("Cannot erase element - already empty polynomial.");
        }

        --size;

        delete it.current;
    }

    template<class C>
    void DoubleEndedPolynomial<C>::erase(DoubleEndedPolynomial<C>::ReverseIterator it) {
        erase(std::prev(it.base()));
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popBack() {
        erase(std::prev(end()));
    }

    template<class C>
    void DoubleEndedPolynomial<C>::popFront() {
        erase(begin());
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::front() {
        return *begin();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType &DoubleEndedPolynomial<C>::back() {
        return *(std::prev(end()));
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType DoubleEndedPolynomial<C>::front() const {
        return *begin();
    }

    template<class C>
    typename DoubleEndedPolynomial<C>::CoefficientType DoubleEndedPolynomial<C>::back() const {
        return *std::prev(end());
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
