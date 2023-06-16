#ifndef QMCHRONMAP_H
#define QMCHRONMAP_H

#include <QDebug>
#include <QHash>

#include <list>

template <class K, class T>
class QMChronMap {
private:
    std::list<QPair<K, T>> m_list;
    QHash<K, typename decltype(m_list)::iterator> m_map;

public:
    typedef T mapped_type;
    typedef K key_type;
    typedef typename decltype(m_list)::difference_type difference_type;
    typedef int size_type;

    QMChronMap() {
    }

    QMChronMap(const QMChronMap &other) {
        for (const auto &item : other.m_list) {
            append(item.first, item.second);
        }
    }

    QMChronMap(QMChronMap &&other) noexcept {
        m_list = std::move(other.m_list);
        m_map = std::move(other.m_map);
    }

    QMChronMap &operator=(const QMChronMap &other) {
        clear();
        for (const auto &item : other.m_list) {
            append(item.first, item.second);
        }
        return *this;
    }

    QMChronMap &operator=(QMChronMap &&other) noexcept {
        m_list = std::move(other.m_list);
        m_map = std::move(other.m_map);
        return *this;
    }

    QMChronMap(std::initializer_list<std::pair<K, T>> list) {
        for (typename std::initializer_list<std::pair<K, T>>::const_iterator it = list.begin(); it != list.end(); ++it)
            append(it->first, it->second);
    }

    template <typename InputIterator>
    QMChronMap(InputIterator f, InputIterator l) {
        for (; f != l; ++f)
            append(f.key(), f.value());
    }

    inline bool operator==(const QMChronMap &other) const {
        return m_list == other.m_list;
    }

    inline bool operator!=(const QMChronMap &other) const {
        return m_list != other.m_list;
    }

    class iterator {
    public:
        iterator() = default;

        typedef typename decltype(m_list)::iterator::iterator_category iterator_category;
        typedef typename decltype(m_list)::iterator::difference_type difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline const K &key() const {
            return i->first;
        }
        inline T &value() const {
            return i->second;
        }
        inline T &operator*() const {
            return value();
        }
        inline T *operator->() const {
            return &value();
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

        friend class QMChronMap;
        friend class const_iterator;
    };

    class const_iterator {
    public:
        const_iterator() = default;
        const_iterator(const iterator &it) : i(it.i) {
        }

        typedef typename decltype(m_list)::const_iterator::iterator_category iterator_category;
        typedef typename decltype(m_list)::const_iterator::difference_type difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline const K &key() const {
            return i->first;
        }
        inline const T &value() const {
            return i->second;
        }
        inline const T &operator*() const {
            return value();
        }
        inline const T *operator->() const {
            return &value();
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

        friend class QMChronMap;
    };

    QPair<iterator, bool> append(const K &key, const T &val, bool replace = true) {
        iterator tmp;
        if (tryReplace(key, val, &tmp, replace)) {
            return {tmp, false};
        }
        auto it = m_list.insert(m_list.end(), qMakePair(key, val));
        m_map.insert(key, it);
        return {iterator(it), true};
    }

    QPair<iterator, bool> prepend(const K &key, const T &val, bool replace = true) {
        iterator tmp;
        if (tryReplace(key, val, &tmp, replace)) {
            return {tmp, false};
        }
        auto it = m_list.insert(m_list.begin(), qMakePair(key, val));
        m_map.insert(key, it);
        return {iterator(it), true};
    }

    QPair<iterator, bool> insert(const const_iterator &it, const K &key, const T &val, bool replace = true) {
        iterator tmp;
        if (tryReplace(key, val, &tmp, replace)) {
            return {tmp, false};
        }
        auto it2 = m_list.insert(it.i, qMakePair(key, val));
        m_map.insert(key, it2);
        return {iterator(it2), true};
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
        auto it2 = m_map.find(it.key());
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

    T value(const K &key, const T &defaultValue = T{}) const {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            return (*it.value()).second;
        }
        return defaultValue;
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

    QList<K> keys() const {
        QList<K> res;
        for (const auto &item : qAsConst(m_list)) {
            res.append(item.first);
        }
        return res;
    }

    QList<T> values() const {
        QList<T> res;
        for (const auto &item : qAsConst(m_list)) {
            res.append(item.second);
        }
        return res;
    }
    int capacity() const {
        return m_map.capacity();
    }
    void reserve(int size) {
        m_map.reserve(size);
    }

private:
    bool tryReplace(const K &key, const T &val, iterator *it, bool noDryRun) {
        auto it0 = m_map.find(key);
        if (it0 != m_map.end()) {
            if (noDryRun)
                it0.value()->second = val;
            *it = iterator(it0.value());
            return true;
        }
        return false;
    }
};

template <class Key, class T>
inline QDebug operator<<(QDebug debug, const QMChronMap<Key, T> &map) {
    const bool oldSetting = debug.autoInsertSpaces();
    debug.nospace() << "QMChronMap(";
    for (typename QMChronMap<Key, T>::const_iterator it = map.constBegin(); it != map.constEnd(); ++it) {
        debug << '(' << it.key() << ", " << it.value() << ')';
    }
    debug << ')';
    debug.setAutoInsertSpaces(oldSetting);
    return debug.maybeSpace();
}


#endif // QMCHRONMAP_H
