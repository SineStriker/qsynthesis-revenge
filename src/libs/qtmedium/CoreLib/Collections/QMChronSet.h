#ifndef QMCHRONSET_H
#define QMCHRONSET_H

#include <list>

#include <QHash>

template <class K>
class QMChronSet {
private:
    std::list<K> m_list;
    QHash<K, typename decltype(m_list)::iterator> m_map;

public:
    class iterator {
    public:
        iterator() = default;

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

    iterator insert(const K &key) {
        auto it = m_list.insert(m_list.end(), key);
        m_map.insert(key, it);
        return iterator(it);
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

    void erase(const iterator &it) {
        return erase(const_iterator(it));
    }

    void erase(const const_iterator &it) {
        auto it2 = m_map.find(*it);
        if (it2 == m_map.end()) {
            return;
        }
        m_list.erase(it2.value());
        m_map.erase(it2);
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
};

template <class K, class T>
class QMChronMap {
private:
    std::list<QPair<K, T>> m_list;
    QHash<K, typename decltype(m_list)::iterator> m_map;

public:
    class iterator {
    public:
        iterator() = default;

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

    iterator insert(const K &key, const T &val) {
        auto it = m_list.insert(m_list.end(), qMakePair(key, val));
        m_map.insert(key, it);
        return iterator(it);
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

    void erase(const iterator &it) {
        return erase(const_iterator(it));
    }

    void erase(const const_iterator &it) {
        auto it2 = m_map.find(it.key());
        if (it2 == m_map.end()) {
            return;
        }
        m_list.erase(it2.value());
        m_map.erase(it2);
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

    QList<K> keys() const {
        QList<K> res;
        for (const auto &item : qAsConst(m_list)) {
            res.append(item.first);
        }
        return res;
    }

    QList<T> values() const {
        return {m_list.begin(), m_list.end()};
    }
};

#endif // QMCHRONSET_H
