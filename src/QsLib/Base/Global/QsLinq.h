#ifndef QSLINQ_H
#define QSLINQ_H

#include <QHash>
#include <QList>
#include <QMap>

namespace QsLinq {

    /**
     * @brief C# IEnumerable.Where
     *
     */
    template <class T, class Selector>
    QList<T> Where(const QList<T> &list, Selector selector) {
        QList<T> res;
        for (const auto &item : qAsConst(list)) {
            if (selector(item)) {
                res.append(item);
            }
        }
        return res;
    }

    /**
     * @brief C# IEnumerable.Select
     *
     */
    template <class T, class V, class Mapper>
    QList<V> Select(const QList<T> &list, Mapper mapper) {
        QList<V> res;
        res.reserve(list.size());
        for (const auto &item : qAsConst(list)) {
            res.append(mapper(item));
        }
        return res;
    }

    template <class K, class V, class V2, class Mapper>
    QList<V2> Select(const QMap<K, V> &map, Mapper mapper) {
        QList<V2> res;
        res.reserve(map.size());
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.append(mapper(it.key(), it.value()));
        }
        return res;
    }

    /**
     * @brief C# IEnumerable.Any
     *
     */
    template <class T, class Validator>
    bool Any(const QList<T> &list, Validator validator) {
        for (const auto &item : qAsConst(list)) {
            if (validator(item)) {
                return true;
            }
        }
        return false;
    }

    template <class T, class Validator>
    bool All(const QList<T> &list, Validator validator) {
        for (const auto &item : qAsConst(list)) {
            if (!validator(item)) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief C# IEnumerable.ForEach
     *
     */
    template <class T, class Func>
    void ForEach(const QList<T> &list, Func func) {
        for (const auto &item : qAsConst(list)) {
            func(item);
        }
    }

    template <class K, class T>
    QHash<K, T> MapToHash(const QMap<K, T> &map) {
        QHash<K, T> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), it.value());
        }
        return res;
    }

    template <class K, class T>
    QMap<K, T> HashToMap(const QHash<K, T> &map) {
        QMap<K, T> res;
        for (auto it = map.begin(); it != map.end(); ++it) {
            res.insert(it.key(), it.value());
        }
        return res;
    }

} // namespace QsLinq

#endif // QSLINQ_H
