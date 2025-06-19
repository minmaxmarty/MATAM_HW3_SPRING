#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
        class Node;

        Node* m_head;
        Node* m_tail;
        unsigned int m_size;

        void clear(Node* headToDelete);
        void copyList(Node *&newHead, Node *&newTail, const SortedList& other);

    public:

        // constructors

        SortedList();

        SortedList(const SortedList& other);

        ~SortedList();

        SortedList& operator=(const SortedList& other);

        // iterator

        class ConstIterator;

        ConstIterator begin() const;

        ConstIterator end() const;

        // methods

        SortedList &insert(const T &newData);

        SortedList &remove(const ConstIterator &givenIt);

        int length() const;

        template <typename Function>
        SortedList filter(Function filterFunction) const;

        template <typename Function>
        SortedList apply(Function applyFunction) const;


        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */

    };

    template<typename T>
    class SortedList<T>::Node {
        friend SortedList;

        T m_data;
        Node* m_next;
        Node* m_prev;

        // constructor
        explicit Node(const T& data, Node* next = nullptr, Node* prev = nullptr);
        ~Node() = default;

    };

    template <class T>
    class SortedList<T>::ConstIterator {
        friend SortedList;

        Node* m_currentNode;

        // private constructors
        explicit ConstIterator(Node* node);

    public:

        ConstIterator(const ConstIterator& other) = default;
        ConstIterator& operator=(const ConstIterator& other) = default;
        ~ConstIterator() = default;

        const T& operator*() const; // unary operator
        ConstIterator& operator++();
        bool operator!=(const ConstIterator& other) const;

    /**
     * the class should support the following public interface:
     * if needed, use =defualt / =delete
     *
     * constructors and destructor:
     * 1. a ctor(or ctors) your implementation needs
     * 2. copy constructor
     * 3. operator= - assignment operator
     * 4. ~ConstIterator() - destructor
     *
     * operators:
     * 5. operator* - returns the element the iterator points to
     * 6. operator++ - advances the iterator to the next element
     * 7. operator!= - returns true if the iterator points to a different element
     *
     */
    };

    // ------------------------------- SortedList ------------------------------- //

    template <typename T>
    SortedList<T>::SortedList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    template<typename T>
    SortedList<T>::SortedList(const SortedList &other) : m_head(nullptr), m_tail(nullptr), m_size(other.m_size) {
        copyList(m_head, m_tail, other);
    }

    template<typename T>
    SortedList<T>::~SortedList() {
        clear(m_head);
        m_head = nullptr;
        m_tail = nullptr;
    }

    template <typename T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& other) {
        if (this == &other) { // if they are the same
            return *this;
        }
        // initialize new head, tail and size
        Node* newHead = nullptr;
        Node* newTail = nullptr;
        unsigned int newSize = other.m_size;

        copyList(newHead, newTail, other); // copy the list into the new parameters

        clear(m_head); // delete the old list

        // make this the new list
        m_head = newHead;
        m_tail = newTail;
        m_size = newSize;

        return *this;
    }

    // methods

    template<typename T>
    SortedList<T> &SortedList<T>::insert(const T &newData) {
        if (m_head == nullptr) { // the list is empty
            m_head = m_tail = new Node(newData, nullptr, nullptr);
        }
        else if (newData > m_head->m_data) { // insert into the first spot
            Node* newNode = new Node(newData, m_head, nullptr);
            m_head->m_prev = newNode;
            m_head = newNode;
        }
        else if (!(newData > m_tail->m_data)) { // insert into the last spot
            Node* newNode = new Node(newData, nullptr, m_tail);
            m_tail->m_next = newNode;
            m_tail = newNode;
        }
        else { // find where to insert
            for (ConstIterator It = begin(); It != end(); ++It) {
                if (!(newData > *It) && newData > It.m_currentNode->m_next->m_data) {
                    Node* newNode = new Node(newData, It.m_currentNode->m_next, It.m_currentNode);
                    It.m_currentNode->m_next = newNode;
                    newNode->m_next->m_prev = newNode;
                    break;
                }
            }
        }

        m_size++;

        return *this;
    }

    template<typename T>
    SortedList<T> &SortedList<T>::remove(const ConstIterator &givenIt) {
        Node* victim = givenIt.m_currentNode;
        if (victim == nullptr) {
            return *this;
        }

        Node* prev = victim->m_prev;
        Node* next = victim->m_next;
        // if prev exists connect it to the next;
        if (prev != nullptr) {
            prev->m_next = next;
        } // if not, than it's the head
        else {
            m_head = next;
        }
        // similarly to the prev logic
        if (next != nullptr) {
            next->m_prev = prev;
        }
        else {
            m_tail = prev;
        }
        // delete what you want to remove
        delete victim;
        // decrease size
        m_size--;

        return *this;
    }

    template<typename T>
    int SortedList<T>::length() const {
        return m_size;
    }

    template<typename T>
    template<typename Function>
    SortedList<T> SortedList<T>::filter(Function filterFunction) const {
        SortedList newList;
        for (ConstIterator It = begin(); It != end(); ++It) {
            if (filterFunction(*It)) {
                newList.insert(*It);
            }
        }

        return newList;
    }

    template<typename T>
    template<typename Function>
    SortedList<T> SortedList<T>::apply(Function applyFunction) const {
        SortedList newList;
        for (ConstIterator It = begin(); It != end(); ++It) {
            Node* curNode = It.m_currentNode;
            newList.insert(applyFunction(curNode->m_data));
        }

        return newList;
    }

    // methods for ConstIterator inside sortedList

    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        return ConstIterator(m_head);
    }

    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(nullptr);
    }

    // ---------------------------------- Node ---------------------------------- //

    template <typename T>
    SortedList<T>::Node::Node(const T& data, Node* next, Node* prev) : m_data(data), m_next(next), m_prev(prev) {}

    // -------------------------------- Iterator -------------------------------- //

    // constructors

    template <typename T>
    SortedList<T>::ConstIterator::ConstIterator(Node *node) : m_currentNode(node) {}

    // operators

    template <typename T>
    const T& SortedList<T>::ConstIterator::operator*() const {
        if (m_currentNode == nullptr) {
            throw std::out_of_range("No data"); // incase we are out of range
        }
        return m_currentNode->m_data; // return the data inside the node that the iterator is pointing to
    }

    template <typename T>
    typename SortedList<T>::ConstIterator& SortedList<T>::ConstIterator::operator++() {
        if (m_currentNode == nullptr) {
            throw std::out_of_range("Out of range");
        }
        m_currentNode = m_currentNode->m_next;
        return *this;
    }

    template <typename T>
    bool SortedList<T>::ConstIterator::operator!=(const ConstIterator& other) const {
        return m_currentNode != other.m_currentNode;
    }

    // ---------------------------------- Helper ---------------------------------- //

    template<typename T>
    void SortedList<T>::clear(Node* headToDelete) {
        if (headToDelete == nullptr) {
            return;
        }

        Node* cur = headToDelete;
        while (cur) {
            Node* toDelete = cur;
            cur = cur->m_next;
            delete toDelete;
        }
    }

    template<typename T>
    void SortedList<T>::copyList(Node *&newHead, Node *&newTail, const SortedList& other) {
        try { // if an allocation fails
            Node* prev = nullptr;
            for (ConstIterator It = other.begin(); It != other.end(); ++It) {
                Node* newNode = new Node(*It, nullptr, nullptr);
                if (newHead == nullptr) {
                    newHead = newNode;
                }
                else {
                    prev->m_next = newNode;
                    prev->m_next->m_prev = prev;
                }
                prev = newNode;
            }
            newTail = prev;
        }
        catch (...) { // delete what has been allocated so far
            clear(newHead);
            throw;
        }
    }
}

