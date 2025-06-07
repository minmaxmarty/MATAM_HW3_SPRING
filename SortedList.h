#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
    private:
        class m_node;
        Node* m_head;
        Node* m_tail;

    public:

        SortedList();

        SortedList(const SortedList& other);

        ~SortedList();

        SortedList& operator=(const SortedList& other);

        class ConstIterator;

        ConstIterator begin() const;

        ConstIterator end() const;




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
    class SortedList<T>::m_node {
    private:
        T m_data;
        m_node* m_next = nullptr;
        m_node* m_prev = nullptr;
    };

    template <class T>
    class SortedList<T>::ConstIterator {

    private:
        SortedList* m_SortedList;
        int m_index;
        ConstIterator(SortedList* Sortedlist, const int& index) : m_SortedList(Sortedlist), m_index(index) {} // private constructor
        ConstIterator(const ConstIterator& other);
        ConstIterator& operator=(const ConstIterator& other);
        ~ConstIterator();

    public:

        T& operator*(); // unary operator
        ConstIterator& operator++();
        bool operator!=(const ConstIterator& other);

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




}

