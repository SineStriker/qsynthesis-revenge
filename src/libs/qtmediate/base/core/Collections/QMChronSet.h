#ifndef QMCHRONSET_H
#define QMCHRONSET_H

#include <QDebug>
#include <QHash>

#include <list>

template <class K>
class QMChronSet {
private:
    std::list<K> m_list;
    QHash<K, typename decltype(m_list)::iterator> m_map;

public:
    typedef K key_type;
    typedef K value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename decltype(m_list)::difference_type difference_type;
    typedef int size_type;

    QMChronSet() {
    }

    QMChronSet(const QMChronSet &other) {
        for (const auto &item : other.m_list) {
            append(item);
        }
    }

    QMChronSet(QMChronSet &&other) noexcept {
        m_list = std::move(other.m_list);
        m_map = std::move(other.m_map);
    }

    QMChronSet &operator=(const QMChronSet &other) {
        clear();
        for (const auto &item : other.m_list) {
            append(item);
        }
        return *this;
    }

    QMChronSet &operator=(QMChronSet &&other) noexcept {
        m_list = std::move(other.m_list);
        m_map = std::move(other.m_map);
        return *this;
    }

    QMChronSet(std::initializer_list<K> list) : QMChronSet(list.begin(), list.end()) {
    }

    template <typename InputIterator>
    QMChronSet(InputIterator first, InputIterator last) {
        for (; first != last; ++first)
            append(*first);
    }

    inline bool operator==(const QMChronSet &other) const {
        return m_list == other.m_list;
    }

    inline bool operator!=(const QMChronSet &other) const {
        return m_list != other.m_list;
    }

    class iterator {
    public:
        iterator() = default;

        typedef typename decltype(m_list)::iterator::iterator_category iterator_category;
        typedef typename decltype(m_list)::iterator::difference_type difference_type;
        typedef K value_type;
        typedef const K *pointer;
        typedef const K &reference;

        inline K &operator*() const {
            return *i;
        }
        inline K *operator->() const {
            return &(*i);
        }
        inline bool operator==(const iterator &o) const {
            return i == o.i;
        }
        inline bool operator!=(const iterator &o) const {
            return i != o.i;
        }
        inline iterator &operator++() {
            i++;
            return *this;
        }
        inline iterator operator++(int) {
            iterator r = *this;
            i++;
            return r;
        }
        inline iterator &operator--() {
            i--;
            return *this;
        }
        inline iterator operator--(int) {
            iterator r = *this;
            i--;
            return r;
        }

    private:
        explicit iterator(const typename decltype(m_list)::iterator &i) : i(i) {
        }

        typename decltype(m_list)::iterator i;

        friend class QMChronSet;
        friend class const_iterator;
    };

    class const_iterator {
    public:
        const_iterator() = default;
        const_iterator(const iterator &it) : i(it.i) {
        }

        typedef typename decltype(m_list)::const_iterator::iterator_category iterator_category;
        typedef typename decltype(m_list)::const_iterator::difference_type difference_type;
        typedef K value_type;
        typedef const K *pointer;
        typedef const K &reference;

        inline const K &operator*() const {
            return *i;
        }
        inline const K *operator->() const {
            return &(*i);
        }
        inline bool operator==(const const_iterator &o) const {
            return i == o.i;
        }
        inline bool operator!=(const const_iterator &o) const {
            return i != o.i;
        }
        inline const_iterator &operator++() {
            i++;
            return *this;
        }
        inline const_iterator operator++(int) {
            iterator r = *this;
            i++;
            return r;
        }
        inline const_iterator &operator--() {
            i--;
            return *this;
        }
        inline const_iterator operator--(int) {
            iterator r = *this;
            i--;
            return r;
        }

    private:
        explicit const_iterator(const typename decltype(m_list)::const_iterator &i) : i(i) {
        }

        typename decltype(m_list)::const_iterator i;

        friend class QMChronSet;
    };

    QPair<iterator, bool> append(const K &key) {
        iterator tmp;
        if (tryReplace(key, &tmp)) {
            return {tmp, false};
        }
        auto it = m_list.insert(m_list.end(), key);
        m_map.insert(key, it);
        return qMakePair(iterator(it), true);
    }

    QPair<iterator, bool> prepend(const K &key) {
        iterator tmp;
        if (tryReplace(key, &tmp)) {
            return {tmp, false};
        }
        auto it = m_list.insert(m_list.begin(), key);
        m_map.insert(key, it);
        return qMakePair(iterator(it), true);
    }

    QPair<iterator, bool> insert(const const_iterator &it, const K &key) {
        iterator tmp;
        if (tryReplace(key, &tmp)) {
            return {tmp, false};
        }
        auto it2 = m_list.insert(it.i, key);
        m_map.insert(key, it2);
        return qMakePair(iterator(it2), true);
    }

    bool remove(const K &key) {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            return false;
        }
        m_list.erase(it.value());
        m_map.erase(it);
        return true;
    }

    iterator erase(const iterator &it) {
        return erase(const_iterator(it));
    }

    iterator erase(const const_iterator &it) {
        auto it2 = m_map.find(*it);
        if (it2 == m_map.end()) {
            return iterator();
        }
        auto res = std::next(it2.value());
        m_list.erase(it2.value());
        m_map.erase(it2);
        return iterator(res);
    }

    iterator find(const K &key) {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            return iterator(it.value());
        }
        return end();
    }

    const_iterator find(const K &key) const {
        return constFind(key);
    }

    const_iterator constFind(const K &key) const {
        auto it = m_map.constFind(key);
        if (it != m_map.cend()) {
            return const_iterator(it.value());
        }
        return cend();
    }

    inline iterator begin() {
        return iterator(m_list.begin());
    }
    inline const_iterator begin() const {
        return const_iterator(m_list.cbegin());
    }
    inline const_iterator cbegin() const {
        return const_iterator(m_list.cbegin());
    }
    inline const_iterator constBegin() const {
        return const_iterator(m_list.cbegin());
    }
    inline iterator end() {
        return iterator(m_list.end());
    }
    inline const_iterator end() const {
        return const_iterator(m_list.cend());
    }
    inline const_iterator cend() const {
        return const_iterator(m_list.cend());
    }
    inline const_iterator constEnd() const {
        return const_iterator(m_list.cend());
    }
    bool contains(const K &key) const {
        return m_map.find(key) != m_map.end();
    }
    int size() const {
        return m_list.size();
    }
    bool isEmpty() const {
        return m_list.empty();
    }
    void clear() {
        m_list.clear();
        m_map.clear();
    }
    QList<K> values() const {
        return {m_list.begin(), m_list.end()};
    }
    int capacity() const {
        return m_map.capacity();
    }
    void reserve(int size) {
        m_map.reserve(size);
    }

private:
    bool tryReplace(const K &key, iterator *it) {
        auto it0 = m_map.find(key);
        if (it0 != m_map.end()) {
            *it = iterator(it0.value());
            return true;
        }
        return false;
    }
};

template <typename T>
inline QDebug operator<<(QDebug debug, const QMChronSet<T> &set) {
    return QtPrivate::printSequentialContainer(debug, "QMChronSet", set);
}

#endif // QMCHRONSET_H
