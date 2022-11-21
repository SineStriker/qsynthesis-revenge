#ifndef __QLINKEDNODE_H__
#define __QLINKEDNODE_H__

#include <QScopedPointer>
#include <QString>

template <class T> // T must have copy constructor
class QLinkedNode {
public:
    QLinkedNode();
    QLinkedNode(const T &data);
    ~QLinkedNode();

    QLinkedNode *left() const;
    QLinkedNode *right() const;

    void insertLeft(QLinkedNode *node);  // insert node to the left of this
    void insertRight(QLinkedNode *node); // insert node to the right of this

    bool isLeftmost() const;
    bool isRightmost() const;

    void take();

    T data() const;
    void setData(const T &data);

    class Sentry {
    public:
        Sentry();
        ~Sentry();

        int size() const;
        bool isEmpty() const;

        void append(QLinkedNode *node);
        void prepend(QLinkedNode *node);

        QLinkedNode *first() const;
        QLinkedNode *last() const;

    private:
        QLinkedNode _head;
        QLinkedNode _tail;

        int _count;

        friend class QLinkedNode;
    };

private:
    QLinkedNode *_left;
    QLinkedNode *_right;

    Sentry *_sentry;

    enum Type {
        Head,
        Tail,
        Data,
    };

    Type _type;
    QScopedPointer<T> _data;

    // Constructor for creating head and tail
    QLinkedNode(Type t, Sentry *sentry);
};

template <class T>
QLinkedNode<T>::QLinkedNode() : _type(Data) {
    _left = _right = nullptr;
    _sentry = nullptr;
}

template <class T>
QLinkedNode<T>::QLinkedNode(const T &data) : QLinkedNode(), _data(new T(data)) {
}

template <class T>
QLinkedNode<T>::~QLinkedNode() {
    if (_type == Data)
        take();
}

template <class T>
QLinkedNode<T> *QLinkedNode<T>::left() const {
    Q_ASSERT(_type == Data);
    return _left->_is_data ? _left : nullptr;
}

template <class T>
QLinkedNode<T> *QLinkedNode<T>::right() const {
    Q_ASSERT(_type == Data);
    return _right->_is_data ? _right : nullptr;
}

template <class T>
void QLinkedNode<T>::insertLeft(QLinkedNode *node) {
    if (this->_left == node && node->_right == this) {
        return;
    }
    Q_ASSERT(_sentry);

    node->take();

    node->_left = this->_left;
    node->_right = this;

    if (this->_left)
        this->_left->_right = node;
    this->_left = node;

    node->_sentry = this->_sentry;
    _sentry->_count++;
}


template <class T>
void QLinkedNode<T>::insertRight(QLinkedNode *node) {
    if (this->_right == node && node->_left == this) {
        return;
    }
    Q_ASSERT(_sentry);

    node->take();

    node->_left = this;
    node->_right = this->_right;

    if (this->_right)
        this->_right->_left = node;
    this->_right = node;

    node->_sentry = this->_sentry;
    _sentry->_count++;
}

template <class T>
bool QLinkedNode<T>::isLeftmost() const {
    Q_ASSERT(_type == Data);
    return _left->_type != Data;
}

template <class T>
bool QLinkedNode<T>::isRightmost() const {
    Q_ASSERT(_type == Data);
    return _right->_type != Data;
}

template <class T>
void QLinkedNode<T>::take() {
    Q_ASSERT(_type == Data);
    if (_sentry) {
        Q_ASSERT(_left && _right);

        _left->_right = _right;
        _right->_left = _left;
        _sentry->_count--;

        _left = nullptr;
        _right = nullptr;
        _sentry = nullptr;
    }
}

template <class T>
T QLinkedNode<T>::data() const {
    Q_ASSERT(_type == Data);
    return *_data.data();
}

template <class T>
void QLinkedNode<T>::setData(const T &data) {
    Q_ASSERT(_type == Data);
    _data.reset(new T(data));
}

template <class T>
QLinkedNode<T>::Sentry::Sentry() : _head(QLinkedNode::Head, this), _tail(QLinkedNode::Tail, this) {
    _head._right = &_tail;
    _tail._left = &_head;
    _count = 0;
}

template <class T>
QLinkedNode<T>::Sentry::~Sentry() {
}

template <class T>
int QLinkedNode<T>::Sentry::size() const {
    return _count;
}

template <class T>
bool QLinkedNode<T>::Sentry::isEmpty() const {
    return _head._right == &_tail;
}

template <class T>
void QLinkedNode<T>::Sentry::append(QLinkedNode *node) {
    _tail.insertLeft(node);
}

template <class T>
void QLinkedNode<T>::Sentry::prepend(QLinkedNode *node) {
    _head.insertRight(node);
}

template <class T>
QLinkedNode<T> *QLinkedNode<T>::Sentry::first() const {
    return _head._right->_is_data ? _head._right : nullptr;
}

template <class T>
QLinkedNode<T> *QLinkedNode<T>::Sentry::last() const {
    return _tail._left->_is_data ? _tail._left : nullptr;
}

template <class T>
QLinkedNode<T>::QLinkedNode(Type t, Sentry *sentry) : _type(t) {
    _left = _right = nullptr;
    _sentry = sentry;
}


#endif // __QLINKEDNODE_H__
