#ifndef QMMAPTRANSACTION_H
#define QMMAPTRANSACTION_H

#include <QDebug>
#include <QMap>

template <template <class K, class V> class _CURRENT_MAP, class Key, class T>
class QMapTransaction {
public:
    using key_type = typename _CURRENT_MAP<Key, T>::key_type;
    using mapped_type = typename _CURRENT_MAP<Key, T>::mapped_type;
    using size_type = typename _CURRENT_MAP<Key, T>::size_type;

    QMapTransaction(_CURRENT_MAP<Key, T> *ref) : ref(ref) {
    }

    ~QMapTransaction() {
        if (!changes.empty()) {
            rollBack();
        }
    }

    void rollBack() {
        for (auto it = changes.rbegin(); it != changes.rend(); ++it) {
            const Change &ch = *it;
            if (ch.type == Change::Insert) {
                ref->insert(ch.key, ch.value);
            } else {
                ref->remove(ch.key);
            }
        }
        changes.clear();
    }

    void abandon() {
        changes.clear();
    }

    void clear() {
        for (auto it = ref->begin(); it != ref->end(); ++it) {
            changes.push_back(Change{Change::Insert, it.key(), it.value()});
        }
    }

    int remove(const Key &key) {
        Change ch;
        if (searchBeforeRemove(key, &ch)) {
            changes.push_back(ch);
        }
        return ref->remove(key);
    }

    T take(const Key &key) {
        Change ch;
        if (searchBeforeRemove(key, &ch)) {
            changes.push_back(ch);
        }
        return ref->take(key);
    }

    using iterator = typename _CURRENT_MAP<Key, T>::iterator;
    using const_iterator = typename _CURRENT_MAP<Key, T>::const_iterator;

    iterator erase(iterator it) {
        changes.push_back(Change{Change::Insert, it.key(), it.value()});
        return ref->erase(it);
    }

    iterator insert(const Key &key, const T &value) {
        Change ch;
        searchBeforeRemove(key, &ch);
        changes.push_back(ch);
        return ref->insert(key, value);
    }

    iterator insert(const_iterator pos, const Key &key, const T &value) {
        Change ch;
        searchBeforeRemove(key, &ch);
        changes.push_back(ch);
        return ref->insert(pos, key, value);
    }

    void insert(const _CURRENT_MAP<Key, T> &map) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            insert(it.key(), it.value());
        }
    }

private:
    _CURRENT_MAP<Key, T> *ref;

    struct Change {
        enum Type {
            Insert,
            Remove,
        };
        Type type;
        Key key;
        T value;
    };

    std::list<Change> changes;

    bool searchBeforeRemove(const Key &key, Change *out) {
        auto it = ref->find(key);
        out->type = Change::Remove;
        out->key = key;
        if (it != ref->end()) {
            out->type = Change::Insert;
            out->value = it.value();
            return true;
        }
        return false;
    }
};

#endif // QMMAPTRANSACTION_H
