#ifndef QLINKLIST_H
#define QLINKLIST_H

#include <QSet>

template <class T>
class QLinkList {
    class Node;

public:
    class Iterator {
    public:
        Iterator();

        T &value() const;
        Iterator &operator++();
        Iterator operator++(int);
        Iterator &operator--();
        Iterator operator--(int);

    private:
        Iterator(Node *node);
        Node *node;
        friend class QLinkList;
    };

    class ConstIterator {
    public:
        ConstIterator();
        ConstIterator(const Iterator &it);

        const T &value() const;
        ConstIterator &operator++();
        ConstIterator operator++(int);
        ConstIterator &operator--();
        ConstIterator operator--(int);

    private:
        ConstIterator(Node *node);
        Node *node;
        friend class QLinkList;
    };

    class ReverseIterator {
    public:
        ReverseIterator();

        T &value() const;
        ReverseIterator &operator++();
        ReverseIterator operator++(int);
        ReverseIterator &operator--();
        ReverseIterator operator--(int);

    private:
        ReverseIterator(Node *node);
        Node *node;
        friend class QLinkList;
    };

    class ConstReverseIterator {
    public:
        ConstReverseIterator();
        ConstReverseIterator(const ReverseIterator &it);

        const T &value() const;
        ConstReverseIterator &operator++();
        ConstReverseIterator operator++(int);
        ConstReverseIterator &operator--();
        ConstReverseIterator operator--(int);

    private:
        ConstReverseIterator(Node *node);
        Node *node;
        friend class QLinkList;
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = ReverseIterator;
    using const_reverse_iterator = ConstReverseIterator;

    QLinkList();
    ~QLinkList();

    int size() const;

    ConstIterator begin() const;
    ConstIterator end() const;

    Iterator begin();
    Iterator end();

    ConstReverseIterator rbegin() const;
    ConstReverseIterator rend() const;

    ReverseIterator rbegin();
    ReverseIterator rend();

    Iterator pushBack(T value);
    Iterator pushFront(T value);

    Iterator insertBefore(Iterator it, T value);
    Iterator insertAfter(Iterator it, T value);

    Iterator erase(Iterator it);
    void clear();

private:
    QSet<Node *> nodes;
    Node head, tail;

    class Node {
        T data;

        Node(T data, bool reserved = false);
        ~Node() = default;

        Node *next;
        Node *prev;
        bool reserved;

        friend class QLinkList;
        friend class Iterator;
        friend class ReverseIterator;
    };
};



// Node
template <class T>
QLinkList<T>::Node::Node(T data, bool reserved)
    : data(data), next(nullptr), prev(nullptr), reserved(reserved) {
}

// Iterator
template <class T>
QLinkList<T>::Iterator::Iterator() : node(nullptr) {
}

template <class T>
T &QLinkList<T>::Iterator::value() const {
    return node->data;
}

template <class T>
typename QLinkList<T>::Iterator &QLinkList<T>::Iterator::operator++() {
    if (node) {
        node = node->next;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++this;
    return tmp;
}

template <class T>
typename QLinkList<T>::Iterator &QLinkList<T>::Iterator::operator--() {
    if (node) {
        node = node->prev;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::Iterator::operator--(int) {
    Iterator tmp = *this;
    --this;
    return tmp;
}

template <class T>
QLinkList<T>::Iterator::Iterator(Node *node) : node(node) {
}



// ConstIterator
template <class T>
QLinkList<T>::ConstIterator::ConstIterator() : node(nullptr) {
}

template <class T>
QLinkList<T>::ConstIterator::ConstIterator(const Iterator &it) : node(it.node) {
}

template <class T>
const T &QLinkList<T>::ConstIterator::value() const {
    return node->data;
}

template <class T>
typename QLinkList<T>::ConstIterator &QLinkList<T>::ConstIterator::operator++() {
    if (node) {
        node = node->next;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::ConstIterator QLinkList<T>::ConstIterator::operator++(int) {
    Iterator tmp = *this;
    ++this;
    return tmp;
}

template <class T>
typename QLinkList<T>::ConstIterator &QLinkList<T>::ConstIterator::operator--() {
    if (node) {
        node = node->prev;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::ConstIterator QLinkList<T>::ConstIterator::operator--(int) {
    Iterator tmp = *this;
    --this;
    return tmp;
}

template <class T>
QLinkList<T>::ConstIterator::ConstIterator(Node *node) : node(node) {
}



// ReverseIterator
template <class T>
QLinkList<T>::ReverseIterator::ReverseIterator() : node(nullptr) {
}

template <class T>
T &QLinkList<T>::ReverseIterator::value() const {
    return node->data;
}

template <class T>
typename QLinkList<T>::ReverseIterator &QLinkList<T>::ReverseIterator::operator++() {
    if (node) {
        node = node->prev;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::ReverseIterator QLinkList<T>::ReverseIterator::operator++(int) {
    Iterator tmp = *this;
    ++this;
    return tmp;
}

template <class T>
typename QLinkList<T>::ReverseIterator &QLinkList<T>::ReverseIterator::operator--() {
    if (node) {
        node = node->next;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::ReverseIterator QLinkList<T>::ReverseIterator::operator--(int) {
    Iterator tmp = *this;
    --this;
    return tmp;
}

template <class T>
QLinkList<T>::ReverseIterator::ReverseIterator(Node *node) : node(node) {
}



// ConstReverseIterator
template <class T>
QLinkList<T>::ConstReverseIterator::ConstReverseIterator() : node(nullptr) {
}

template <class T>
QLinkList<T>::ConstReverseIterator::ConstReverseIterator(const ReverseIterator &it)
    : node(it.node) {
}

template <class T>
const T &QLinkList<T>::ConstReverseIterator::value() const {
    return node->data;
}

template <class T>
typename QLinkList<T>::ConstReverseIterator &QLinkList<T>::ConstReverseIterator::operator++() {
    if (node) {
        node = node->prev;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::ConstReverseIterator QLinkList<T>::ConstReverseIterator::operator++(int) {
    Iterator tmp = *this;
    ++this;
    return tmp;
}

template <class T>
typename QLinkList<T>::ConstReverseIterator &QLinkList<T>::ConstReverseIterator::operator--() {
    if (node) {
        node = node->next;
    }
    if (node && node->reserved) {
        node = nullptr;
    }
    return *this;
}

template <class T>
typename QLinkList<T>::ConstReverseIterator QLinkList<T>::ConstReverseIterator::operator--(int) {
    Iterator tmp = *this;
    --this;
    return tmp;
}

template <class T>
QLinkList<T>::ConstReverseIterator::ConstReverseIterator(Node *node) : node(node) {
}



// QLinkList
template <class T>
QLinkList<T>::QLinkList() : head(T{}, true), tail(T{}, true) {
    head.next = &tail;
    tail.prev = &head;
}

template <class T>
QLinkList<T>::~QLinkList() {
    clear();
}

template <class T>
int QLinkList<T>::size() const {
    return nodes.size();
}

template <class T>
typename QLinkList<T>::ConstIterator QLinkList<T>::begin() const {
    if (head.next == &tail) {
        return ConstIterator();
    }
    return ConstIterator(head.next);
}

template <class T>
typename QLinkList<T>::ConstIterator QLinkList<T>::end() const {
    return ConstIterator();
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::begin() {
    if (head.next == &tail) {
        return Iterator();
    }
    return Iterator(head.next);
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::end() {
    return Iterator();
}

template <class T>
typename QLinkList<T>::ConstReverseIterator QLinkList<T>::rbegin() const {
    if (tail.prev == &head) {
        return ConstReverseIterator();
    }
    return ConstReverseIterator(tail.prev);
}

template <class T>
typename QLinkList<T>::ConstReverseIterator QLinkList<T>::rend() const {
    return ConstReverseIterator();
}

template <class T>
typename QLinkList<T>::ReverseIterator QLinkList<T>::rbegin() {
    if (tail.prev == &head) {
        return ReverseIterator();
    }
    return ReverseIterator(tail.prev);
}

template <class T>
typename QLinkList<T>::ReverseIterator QLinkList<T>::rend() {
    return ReverseIterator();
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::pushBack(T value) {
    return insertAfter(Iterator(tail.prev), value);
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::pushFront(T value) {
    return insertBefore(Iterator(head.next), value);
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::insertBefore(Iterator it, T value) {
    Node *node = it.node;
    if (!nodes.contains(node) && node != &tail) {
        return Iterator();
    }
    Node *p = new Node(value);
    if (node->prev) {
        node->prev->next = p;
        p->prev = node->prev;
    }
    p->next = node;
    node->prev = p;
    nodes.insert(p);
    return Iterator(p);
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::insertAfter(Iterator it, T value) {
    Node *node = it.node;
    if (!nodes.contains(node) && node != &head) {
        return Iterator();
    }
    Node *p = new Node(value);
    if (node->next) {
        node->next->prev = p;
        p->next = node->next;
    }
    p->prev = node;
    node->next = p;
    nodes.insert(p);
    return Iterator(p);
}

template <class T>
typename QLinkList<T>::Iterator QLinkList<T>::erase(Iterator it) {
    Node *node = it.node;
    auto it2 = nodes.find(node);
    if (it2 == nodes.end()) {
        return Iterator();
    }
    nodes.erase(it2);

    Iterator res(node->prev);
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    delete node;

    return res;
}

template <class T>
void QLinkList<T>::clear() {
    for (auto node : qAsConst(nodes)) {
        delete node;
    }
    nodes.clear();

    head.next = &tail;
    tail.prev = &head;
}

#endif // QLINKLIST_H
