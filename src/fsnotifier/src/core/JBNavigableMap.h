#ifndef JBNAVIGABLEMAP_H
#define JBNAVIGABLEMAP_H

#include <algorithm>
#include <map>

#include <QDebug>
#include <QMap>

#include "JBNavigableSet.h"

template <class _Kty, class _Ty, class _Pr = std::less<_Kty>,
          class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
class JBNavigableMap : public std::map<_Kty, _Ty, _Pr, _Alloc> {
    // ordered red-black tree of {key, mapped} values, unique keys
public:
    using _Mybase = std::map<_Kty, _Ty, _Pr, _Alloc>;
    using key_type = _Kty;
    using mapped_type = _Ty;
    using key_compare = _Pr;
    using value_compare = typename _Mybase::value_compare;
    using value_type = std::pair<const _Kty, _Ty>;
    using allocator_type = typename _Mybase::allocator_type;
    using size_type = typename _Mybase::size_type;
    using difference_type = typename _Mybase::difference_type;
    using pointer = typename _Mybase::pointer;
    using const_pointer = typename _Mybase::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = typename _Mybase::iterator;
    using const_iterator = typename _Mybase::const_iterator;
    using reverse_iterator = typename _Mybase::reverse_iterator;
    using const_reverse_iterator = typename _Mybase::const_reverse_iterator;

    JBNavigableMap() : _Mybase(key_compare()) {
    }

    explicit JBNavigableMap(const allocator_type &_Al) : _Mybase(key_compare(), _Al) {
    }

    JBNavigableMap(const _Mybase &_Right) : _Mybase(_Right) {
    }

    JBNavigableMap(const _Mybase &_Right, const allocator_type &_Al) : _Mybase(_Right, _Al) {
    }

    explicit JBNavigableMap(const key_compare &_Pred) : _Mybase(_Pred) {
    }

    JBNavigableMap(const key_compare &_Pred, const allocator_type &_Al) : _Mybase(_Pred, _Al) {
    }

    template <class _Iter>
    JBNavigableMap(_Iter _First, _Iter _Last) : _Mybase(key_compare()) {
        insert(_First, _Last);
    }

    template <class _Iter>
    JBNavigableMap(_Iter _First, _Iter _Last, const key_compare &_Pred) : _Mybase(_Pred) {
        insert(_First, _Last);
    }

    template <class _Iter>
    JBNavigableMap(_Iter _First, _Iter _Last, const allocator_type &_Al)
        : _Mybase(key_compare(), _Al) {
        insert(_First, _Last);
    }

    template <class _Iter>
    JBNavigableMap(_Iter _First, _Iter _Last, const key_compare &_Pred, const allocator_type &_Al)
        : _Mybase(_Pred, _Al) {
        insert(_First, _Last);
    }

    JBNavigableMap &operator=(const _Mybase &_Right) {
        _Mybase::operator=(_Right);
        return *this;
    }

    JBNavigableMap(_Mybase &&_Right) : _Mybase(std::move(_Right)) {
    }

    JBNavigableMap(_Mybase &&_Right, const allocator_type &_Al) : _Mybase(std::move(_Right), _Al) {
    }

    JBNavigableMap &operator=(_Mybase &&_Right) {
        _Mybase::operator=(std::move(_Right));
        return *this;
    }

public:
    typedef _Kty K;
    typedef _Ty V;

    JBNavigableMap(const QMap<K, V> &map) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            this->insert(it.key(), it.value());
        }
    }

    JBNavigableMap(QMap<K, V> &&map) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            this->insert(it.key(), it.value());
        }
    }

    JBNavigableMap &operator=(const QMap<K, V> &map) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            this->insert(it.key(), it.value());
        }
        return *this;
    }

    JBNavigableMap &operator=(QMap<K, V> &&map) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            this->insert(it.key(), it.value());
        }
        return *this;
    }

    iterator lower(K key) {
        if (this->empty()) {
            return _Mybase::end();
        }
        auto it = ceiling(key);
        return --it;
    }

    const_iterator lower(K key) const {
        if (this->empty()) {
            return _Mybase::end();
        }
        auto it = ceiling(key);
        return --it;
    }

    iterator floor(K key) {
        if (this->empty()) {
            return _Mybase::end();
        }
        auto it = ceiling(key);
        if (it != this->end() && it->first == key) {
            return it;
        }
        return --it;
    }

    const_iterator floor(K key) const {
        if (this->empty()) {
            return _Mybase::end();
        }
        auto it = ceiling(key);
        if (it != this->end() && it->first == key) {
            return it;
        }
        return --it;
    }

    iterator ceiling(K key) {
        return _Mybase::lower_bound(key);
    }

    const_iterator ceiling(K key) const {
        return _Mybase::lower_bound(key);
    }

    iterator higher(K key) {
        return _Mybase::upper_bound(key);
    }

    const_iterator higher(K key) const {
        return _Mybase::upper_bound(key);
    }

    bool contains(K key) const {
        return this->find(key) != this->end();
    }

    bool isEmpty() const {
        return this->empty();
    }

    size_type remove(K key) {
        return this->erase(key);
    }

    std::pair<iterator, bool> insert(const K &key, const V &val) {
        return _Mybase::insert(std::make_pair(key, val));
    }

    iterator computeIfAbsent(const K &key, const V &newVal) {
        auto it = this->find(key);
        if (it == this->end()) {
            return this->insert(key, newVal).first;
        }
        return it;
    }

    QMap<K, V> toQMap() const {
        QMap<K, V> res;
        for (auto it = this->begin(); it != this->end(); ++it) {
            res.insert(it->first, it->second);
        }
        return res;
    }

    JBNavigableSet<K, _Pr, _Alloc> navigableKeySet() const {
        JBNavigableSet<K, _Pr, _Alloc> res;
        for (auto it = this->begin(); it != this->end(); ++it) {
            res.insert(it->first);
        }
        return res;
    }

    template <class _Kty1, class _Ty1, class _Pr1, class _Alloc1>
    friend QDebug operator<<(QDebug debug, const JBNavigableMap<_Kty1, _Ty1, _Pr1, _Alloc1> &map);
};

template <class _Kty1, class _Ty1, class _Pr1, class _Alloc1>
QDebug operator<<(QDebug debug, const JBNavigableMap<_Kty1, _Ty1, _Pr1, _Alloc1> &map) {
    QDebugStateSaver saver(debug);
    debug.nospace() << "NavigableMap(";
    for (auto it = map.begin(); it != map.end(); ++it) {
        debug.nospace() << "(" << it->first << ", " << it->second << ")";
    }
    debug.noquote().nospace() << ")";
    return debug;
}

#endif // JBNAVIGABLEMAP_H
